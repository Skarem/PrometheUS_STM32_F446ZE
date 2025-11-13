#pragma once

#include "stm32f4xx_hal.h"

class DebugButton
{
public:
  DebugButton()   = default;
  ~DebugButton()  = default;

  void init(GPIO_TypeDef* port, uint16_t pin)
  {
    m_port = port;
    m_pin = pin;
    m_lastStableTime = HAL_GetTick();
  }

  inline bool pressed()
  {
    bool now = (HAL_GPIO_ReadPin(m_port, m_pin) == GPIO_PIN_SET);

    if (now != m_lastState)
    {
      uint32_t nowMs = HAL_GetTick();

      if ((nowMs - m_lastStableTime) >= DEBOUCE_MS)
      {
        m_lastStableTime = nowMs;
        m_lastState = now;

        return now == true;
      }
    }
    return false;
  }

private:
  GPIO_TypeDef* m_port      = nullptr;
  uint16_t      m_pin       = 0;

  bool      m_lastState       = GPIO_PIN_RESET;
  uint32_t  m_lastStableTime  = 0;

  const uint32_t DEBOUCE_MS = 20;
};
