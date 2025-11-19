#pragma once

#include "stm32f4xx_hal.h"

class PWMComplementary
{
public:
  PWMComplementary()  = default;
  ~PWMComplementary() = default;

  void init(TIM_HandleTypeDef* timer, uint32_t channel, GPIO_TypeDef* port, uint16_t pin);

  void update(float dutyCycle);

  void start();
  void stop();

private:
  TIM_HandleTypeDef*  m_timer   = nullptr;
  uint32_t            m_channel = 0;

  GPIO_TypeDef* m_port  = nullptr;
  uint16_t      m_pin   = 0;

  uint32_t m_arr;

  static constexpr float MIN_DUTY_CYCLE = 0.00f;
  static constexpr float MAX_DUTY_CYCLE = 0.57f;

  inline uint32_t roundToInt(float x);
};

