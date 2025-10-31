#include "PWMComplementary.hpp"

void PWMComplementary::init(TIM_HandleTypeDef* timer, uint32_t channel)
{
  m_timer   = timer;
  m_channel = channel;
  m_arr = __HAL_TIM_GET_AUTORELOAD(m_timer);
}

void PWMComplementary::start()
{
  // Enable timer counter first
  __HAL_TIM_ENABLE(m_timer);

  // Start PWM generation
  HAL_TIM_PWM_Start(m_timer, m_channel);
  HAL_TIMEx_PWMN_Start(m_timer, m_channel);

  // Then enable MOE (Main Output Enable) so advanced timer actually drives outputs
  __HAL_TIM_MOE_ENABLE(m_timer);
}

void PWMComplementary::stop()
{
  // Ensure CCR = 0 so last cycle is low
  __HAL_TIM_SET_COMPARE(m_timer, m_channel, 0);

  // Stop PWM generation
  HAL_TIM_PWM_Stop(m_timer, m_channel);
  HAL_TIMEx_PWMN_Stop(m_timer, m_channel);

  // Force pins to configured idle state (Idle = Reset/Low)
  __HAL_TIM_MOE_DISABLE(m_timer);

  // Disable timer counter itself
  __HAL_TIM_DISABLE(m_timer);
}

/*
 * Input:
 * float: dutyCycle (0.0f-1.0f)
 */
void PWMComplementary::update(float dutyCycle)
{
  // Clamp dutyCycle
  if (dutyCycle < MIN_DUTY_CYCLE) dutyCycle = MIN_DUTY_CYCLE;
  if (dutyCycle > MAX_DUTY_CYCLE) dutyCycle = MAX_DUTY_CYCLE;

  const uint32_t ccr = roundToInt(dutyCycle * m_arr);
  __HAL_TIM_SET_COMPARE(m_timer, m_channel, ccr);
}

inline uint32_t PWMComplementary::roundToInt(float x)
{
  return static_cast<uint32_t>(x + 0.5f);
}

