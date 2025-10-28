#pragma once

#include "stm32f4xx_hal.h"

class DigitalOutput
{
public:
  DigitalOutput() = default;
  ~DigitalOutput() = default;

  void init(GPIO_TypeDef* port, uint16_t pin)
  {
    m_port  = port;
    m_pin   = pin;
  }

  void on()
  {
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET);
  }

  void off()
  {
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
  }

private:
  GPIO_TypeDef* m_port;
  uint16_t      m_pin;
};
