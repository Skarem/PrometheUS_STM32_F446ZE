#include "PWM.hpp"

void PWM::init(TIM_HandleTypeDef* timer, uint32_t channel)
{
  m_timer   = timer;
  m_channel = channel;
  m_arr = __HAL_TIM_GET_AUTORELOAD(m_timer);
}

void PWM::start()
{
  HAL_TIM_PWM_Start(m_timer, m_channel);
}

void PWM::stop()
{
  HAL_TIM_PWM_Stop(m_timer, m_channel);
}

/*
 * Input:
 * float: dutyCycle (0.0f-1.0f)
 */
void PWM::update(float dutyCycle)
{
  // Clamp dutyCycle
  if (dutyCycle < MIN_DUTY_CYCLE) dutyCycle = MIN_DUTY_CYCLE;
  if (dutyCycle > MAX_DUTY_CYCLE) dutyCycle = MAX_DUTY_CYCLE;

  const uint32_t ccr = roundToInt(dutyCycle * m_arr);
  __HAL_TIM_SET_COMPARE(m_timer, m_channel, ccr);
}

inline uint32_t PWM::roundToInt(float x)
{
  return static_cast<uint32_t>(x + 0.5f);
}
