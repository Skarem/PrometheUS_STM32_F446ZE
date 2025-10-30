#pragma once

#include "main.h"
#include "DefineConstants.hpp"

typedef struct __attribute__((packed))
{
  float potentiometersPosition[FINGER_COUNT];
  float encodersPosition[FINGER_COUNT];
  float clutchesCurrent[FINGER_COUNT];
  float motorVelocity;
} TelemetryPacket;

class TelemetrySender
{
public:
  TelemetrySender()   = default;
  ~TelemetrySender()  = default;

  void init(UART_HandleTypeDef* huart);

  void send(
      const float potentiometersPosition[FINGER_COUNT],
      const float encodersPosition[FINGER_COUNT],
      const float clutchesCurrent[FINGER_COUNT],
      const float motorVelocity);

private:
  UART_HandleTypeDef* m_huart     = nullptr;
  TelemetryPacket     m_txPacket  = {};

  static constexpr size_t POTENTIOMETERS_POSITION_SIZE = sizeof(m_txPacket.potentiometersPosition);
  static constexpr size_t ENCODERS_POSITION_SIZE       = sizeof(m_txPacket.encodersPosition);
  static constexpr size_t CLUTCHES_CURRENT_SIZE        = sizeof(m_txPacket.clutchesCurrent);

  static constexpr size_t PACKET_SIZE = sizeof(m_txPacket);
};
