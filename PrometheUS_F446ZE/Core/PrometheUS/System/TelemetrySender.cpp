#include "TelemetrySender.hpp"
#include <DefineConstants.hpp>

void TelemetrySender::init(UART_HandleTypeDef* huart)
{
  m_huart = huart;
}

void TelemetrySender::send(const uint32_t timestamp,
          const float pots[FINGER_COUNT],
          const float encs[FINGER_COUNT],
          const float curr[FINGER_COUNT],
          const float motor)
{
  // DMA still busy
  if (m_huart->gState != HAL_UART_STATE_READY) return;

  m_packet.clear();

  // m_packet.header       = TELEMETRY_HEADER;
  m_packet.timestampMs  = timestamp;
  for (size_t i = 0; i < FINGER_COUNT; ++i)
  {
    m_packet.potentiometersPosition[i]  = pots[i];
    m_packet.encodersPosition[i]        = encs[i];
    m_packet.clutchesCurrent[i]         = curr[i];
  }
  m_packet.motorVelocity = motor;

  HAL_UART_Transmit_DMA(m_huart, reinterpret_cast<uint8_t*>(&m_packet), PACKET_SIZE);
}
