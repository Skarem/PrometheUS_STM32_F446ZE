#include "EncoderSampler.hpp"

void EncoderSampler::init(SPI_HandleTypeDef* spi, GPIO_TypeDef* chipSelectPeripheral, uint16_t chipSelectPin)
{
  m_spi = spi;
  m_chipSelectPeripheral = chipSelectPeripheral;
  m_chipSelectPin = chipSelectPin;

  sampleRawValue();
  m_offsetPosition = m_rawEncoderValue;
}

void EncoderSampler::sampleRawValue()
{
  static constexpr uint16_t   TRANSMIT_DUMMY_DATA  = 0x0F0F;
  static constexpr uint16_t   ENCODER_12_BITS_MASK = 0x0FFF;

  HAL_GPIO_WritePin(m_chipSelectPeripheral, m_chipSelectPin, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(m_spi, (uint8_t*)&TRANSMIT_DUMMY_DATA, (uint8_t*)&m_rawEncoderValue, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(m_chipSelectPeripheral, m_chipSelectPin, GPIO_PIN_SET);

  m_rawEncoderValue = m_rawEncoderValue & ENCODER_12_BITS_MASK;
}

float EncoderSampler::convertValue() const
{
  return static_cast<float>(m_rawEncoderValue);
}
