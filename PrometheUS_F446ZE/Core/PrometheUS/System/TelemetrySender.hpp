#pragma once

#include "main.h"
#include "DefineConstants.hpp"

enum class ErrorSource : uint8_t
{
      NONE = 0,
      CLUTCH_TEMPERATURE_1,
      CLUTCH_TEMPERATURE_2,
      CLUTCH_TEMPERATURE_3,
      CLUTCH_CURRENT_1,
      CLUTCH_CURRENT_2,
      CLUTCH_CURRENT_3,
      MOTOR_ERROR_PIN,
      LAST_INDEX
};

struct __attribute__((packed)) TelemetryPacket
{
  // Header
  const uint16_t header = TELEMETRY_HEADER;
  uint32_t timestampMs  = 0;

  // Data
  float potentiometersPosition[FINGER_COUNT]  = { 0 };
  float clutchesTemperature[FINGER_COUNT]     = { 0 };
  float clutchesCurrent[FINGER_COUNT]         = { 0 };
  float motorVelocity                         = 0;

  // Error
  bool    inError     = false;
  uint8_t errorSource = static_cast<uint8_t>(ErrorSource::NONE);

  void clear()
  {
    timestampMs = 0;
    for (int i = 0; i < FINGER_COUNT; i++)
    {
      potentiometersPosition[i] = 0.0f;
      clutchesTemperature[i]    = 0.0f;
      clutchesCurrent[i]        = 0.0f;
    }
    motorVelocity = 0.0f;
  }
};

class TelemetrySender
{
public:
  TelemetrySender()   = default;
  ~TelemetrySender()  = default;

  void init(UART_HandleTypeDef* huart);

  void send(const uint32_t timestamp,
            const float pots[FINGER_COUNT],
            const float temp[FINGER_COUNT],
            const float curr[FINGER_COUNT],
            const float motor,
            const bool inError,
            const uint8_t errorSource);

private:
  UART_HandleTypeDef* m_huart     = nullptr;
  TelemetryPacket     m_packet    = {};

  static constexpr size_t PACKET_SIZE = sizeof(m_packet);
};
