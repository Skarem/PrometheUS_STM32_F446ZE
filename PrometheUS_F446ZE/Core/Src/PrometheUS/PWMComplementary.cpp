#include "PWMComplementary.hpp"

void PWMComplementary::init(TIM_HandleTypeDef* timer, uint32_t channel)
{
  m_timer = timer;
  m_channel = channel;

  HAL_TIM_PWM_Start(m_timer, m_channel);
  HAL_TIMEx_PWMN_Start(m_timer, m_channel);
  m_arr = __HAL_TIM_GET_AUTORELOAD(m_timer);
}

/*
 * Input:
 * float: dutyCycle (0.0f-1.0f)
 */
void PWMComplementary::update(float dutyCycle)
{
  uint32_t ccr = static_cast<uint32_t>(dutyCycle * m_arr);
  __HAL_TIM_SET_COMPARE(m_timer, m_channel, ccr);
}

void PWMComplementary::stop()
{
  HAL_TIM_PWM_Stop(m_timer, m_channel);
  HAL_TIMEx_PWMN_Stop(m_timer, m_channel);
}
