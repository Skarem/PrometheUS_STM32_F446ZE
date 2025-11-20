#pragma once

#include "stm32f4xx_hal.h"

class LED
{
public:
  LED()   = default;
  ~LED()  = default;

  void init(GPIO_TypeDef* port, uint16_t pin)
  {
    m_port  = port;
    m_pin   = pin;
  }

  inline void on()
  {
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
  }

  inline void off()
  {
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET);
  }

  inline void toggle()
  {
    HAL_GPIO_TogglePin(m_port, m_pin);
  }

  inline void pulse()
  {
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET);
  }

private:
  GPIO_TypeDef* m_port  = nullptr;
  uint16_t      m_pin   = 0;
};
