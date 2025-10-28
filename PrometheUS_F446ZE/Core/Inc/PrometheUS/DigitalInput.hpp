#pragma once

#include "stm32f4xx_hal.h"

class DigitalInput
{
public:
  DigitalInput()  = default;
  ~DigitalInput() = default;

  void init(GPIO_TypeDef* port, uint16_t pin)
  {
    m_port = port;
    m_pin = pin;
  }

  bool isOn() const
  {
    GPIO_PinState state = HAL_GPIO_ReadPin(m_port, m_pin);
    bool logical = (state == GPIO_PIN_SET);
    return logical;
  }

private:
  GPIO_TypeDef* m_port;
  uint16_t      m_pin;
};

