#include "TelemetrySender.hpp"

#include <cstring>

void TelemetrySender::init(UART_HandleTypeDef* huart)
{
  m_huart = huart;
}

void TelemetrySender::send(
    const float potentiometersPosition[FINGER_COUNT],
    const float encodersPosition[FINGER_COUNT],
    const float clutchesCurrent[FINGER_COUNT],
    const float motorVelocity)
{
  // DMA still busy
  if (m_huart->gState != HAL_UART_STATE_READY) return;

  memcpy(m_txPacket.potentiometersPosition, potentiometersPosition, POTENTIOMETERS_POSITION_SIZE);
  memcpy(m_txPacket.encodersPosition,       encodersPosition,       ENCODERS_POSITION_SIZE);
  memcpy(m_txPacket.clutchesCurrent,        clutchesCurrent,        CLUTCHES_CURRENT_SIZE);
  m_txPacket.motorVelocity = motorVelocity;

  HAL_UART_Transmit_DMA(m_huart, (uint8_t*)&m_txPacket, PACKET_SIZE);
}
