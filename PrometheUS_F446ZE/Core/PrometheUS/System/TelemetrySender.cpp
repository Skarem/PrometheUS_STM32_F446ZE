#include "TelemetrySender.hpp"
#include <DefineConstants.hpp>

void TelemetrySender::init(UART_HandleTypeDef* huart)
{
  m_huart = huart;
}

void TelemetrySender::send(const uint32_t timestamp,
          const float pots[FINGER_COUNT],
          const float temp[FINGER_COUNT],
          const float curr[FINGER_COUNT],
          const float motor,
          const bool inError,
          const uint8_t errorSource)
{
  // DMA still busy
  if (m_huart->gState != HAL_UART_STATE_READY) return;

  m_packet.clear();
  m_packet.timestampMs = timestamp;
  for (size_t i = 0; i < FINGER_COUNT; ++i)
  {
    m_packet.potentiometersPosition[i]  = pots[i];
    m_packet.clutchesTemperature[i]     = temp[i];
    m_packet.clutchesCurrent[i]         = curr[i];
  }
  m_packet.motorVelocity  = motor;

  m_packet.inError        = inError;
  m_packet.errorSource    = errorSource;

  HAL_UART_Transmit_DMA(m_huart, reinterpret_cast<uint8_t*>(&m_packet), PACKET_SIZE);
}
