#pragma once

#include "main.h"

class EncoderSampler
{
public:

  explicit EncoderSampler(SPI_HandleTypeDef* spi, GPIO_TypeDef* clockSelectPeripheral, uint16_t clockSelectPin);
  ~EncoderSampler() = default;

  void initOffsetPosition();

  void sampleRawValue();
  uint16_t convertValue() const;

private:
  SPI_HandleTypeDef*  m_spi;
  GPIO_TypeDef*       m_clockSelectPeripheral;
  uint16_t            m_clockSelectPin;

  uint16_t m_rawEncoderValue;
  uint16_t m_offsetPosition;
};
