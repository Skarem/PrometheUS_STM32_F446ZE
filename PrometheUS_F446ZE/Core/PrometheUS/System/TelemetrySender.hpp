#pragma once

#include "main.h"
#include "DefineConstants.hpp"

struct __attribute__((packed)) TelemetryPacket
{
  // Header
  const uint16_t header = TELEMETRY_HEADER;
  uint32_t timestampMs  = 0;

  // Data
  float potentiometersPosition[FINGER_COUNT]  = { 0 };
  float encodersPosition[FINGER_COUNT]        = { 0 };
  float clutchesCurrent[FINGER_COUNT]         = { 0 };
  float motorVelocity;

  void clear()
  {
    timestampMs = 0;
    for (int i = 0; i < FINGER_COUNT; i++)
    {
      potentiometersPosition[i] = 0.0f;
      encodersPosition[i]       = 0.0f;
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
            const float encs[FINGER_COUNT],
            const float curr[FINGER_COUNT],
            const float motor);

private:
  UART_HandleTypeDef* m_huart     = nullptr;
  TelemetryPacket     m_packet    = {};

  static constexpr size_t PACKET_SIZE = sizeof(m_packet);
};
