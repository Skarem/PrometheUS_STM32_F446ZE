#pragma once

#include "main.h"

class EncoderSampler
{
public:

  EncoderSampler()  = default;
  ~EncoderSampler() = default;

  void init(SPI_HandleTypeDef* spi, GPIO_TypeDef* chipSelectPeripheral, uint16_t chipSelectPin);

  void sampleRawValue();
  uint16_t convertValue() const;

private:
  SPI_HandleTypeDef*  m_spi;
  GPIO_TypeDef*       m_chipSelectPeripheral;
  uint16_t            m_chipSelectPin;

  uint16_t m_rawEncoderValue  = 0;
  uint16_t m_offsetPosition   = 0;
};
