#include "PWM.hpp"

void PWM::init(TIM_HandleTypeDef* timer, uint32_t channel)
{
  m_timer = timer;
  m_channel = channel;

  HAL_TIM_PWM_Start(m_timer, m_channel);
  m_arr = __HAL_TIM_GET_AUTORELOAD(m_timer);
}

/*
 * Input:
 * float: dutyCycle (0.0f-1.0f)
 */
void PWM::update(float dutyCycle)
{
  uint32_t ccr = static_cast<uint32_t>(dutyCycle * m_arr);
  __HAL_TIM_SET_COMPARE(m_timer, m_channel, ccr);
}

void PWM::stop()
{
  HAL_TIM_PWM_Stop(m_timer, m_channel);
}
