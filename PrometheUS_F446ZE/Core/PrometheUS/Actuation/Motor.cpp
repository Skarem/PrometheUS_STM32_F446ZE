#include "Motor.hpp"

#include "main.h"

Motor::Motor(MotorVelocitySampler &motorVelocitySampler)
  : m_motorVelocitySampler(motorVelocitySampler)
{

}

void Motor::init()
{
  // Motor PWM (Needs to be between 10Hz and 5KHz)
  m_pwm.init(&htim4, TIM_CHANNEL_1);
  m_motorVelocitySampler.init(&hadc3, ADC_CHANNEL_3);

  m_error.init(Motor_Error_GPIO_Port, Motor_Error_Pin);
  m_enable.init(Motor_Enable_GPIO_Port, Motor_Enable_Pin);
}

void Motor::start()
{
  m_enable.on();
  m_pwm.start();
}

void Motor::stop()
{
  // Do not stop sending PWM to motor, just disable pin
  m_enable.off();
}

float Motor::getMotorVelocity()
{
  return m_motorVelocitySampler.convertRawAdcValue();
}

bool Motor::isError()
{
  return m_error.isOn();
}
