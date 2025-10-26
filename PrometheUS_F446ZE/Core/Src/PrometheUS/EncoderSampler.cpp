#include "EncoderSampler.hpp"

EncoderSampler::EncoderSampler(SPI_HandleTypeDef* spi, GPIO_TypeDef* clockSelectPeripheral, uint16_t clockSelectPin)
  : m_spi(spi),
    m_clockSelectPeripheral(clockSelectPeripheral),
    m_clockSelectPin(clockSelectPin),
    m_rawEncoderValue(0)
{

}

void EncoderSampler::initOffsetPosition()
{
  sampleRawValue();
  m_offsetPosition = m_rawEncoderValue;
}

void EncoderSampler::sampleRawValue()
{
  static constexpr uint16_t   TRANSMIT_DUMMY_DATA  = 0x0F0F;
  static constexpr uint16_t   ENCODER_12_BITS_MASK = 0x0FFF;

  HAL_GPIO_WritePin(m_clockSelectPeripheral, m_clockSelectPin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(m_spi, (uint8_t*)&TRANSMIT_DUMMY_DATA, (uint8_t*)&m_rawEncoderValue, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(m_clockSelectPeripheral, m_clockSelectPin, GPIO_PIN_SET);

  // m_rawEncoderValue = m_rawEncoderValue & ENCODER_12_BITS_MASK;
}

uint16_t EncoderSampler::convertValue() const
{
  return m_rawEncoderValue;
}
