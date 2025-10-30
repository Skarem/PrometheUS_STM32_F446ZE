#pragma once

#include "stm32f4xx_hal.h"

class PWM
{
public:
  PWM()   = default;
  ~PWM()  = default;

  void init(TIM_HandleTypeDef* timer, uint32_t channel);

  void update(float dutyCycle);

  void start();
  void stop();

private:
  TIM_HandleTypeDef*  m_timer   = nullptr;
  uint32_t            m_channel = 0;

  const uint32_t m_arr = 999;
};

