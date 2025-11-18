#include "Motor.hpp"

#include "main.h"

#include "DefineConstants.hpp"

Motor::Motor(MotorVelocitySampler* motorVelocitySampler)
  : m_motorVelocitySampler(motorVelocitySampler)
{

}

void Motor::init()
{
  // Motor PWM (Needs to be between 10Hz and 5KHz)
  m_pwm.init(&htim4, TIM_CHANNEL_1);
  m_motorVelocitySampler->init(&hadc3, ADC_CHANNEL_3);

  m_error.init(Motor_Error_GPIO_Port, Motor_Error_Pin);
  m_enable.init(Motor_Enable_GPIO_Port, Motor_Enable_Pin);

  m_pwm.start();
}

void Motor::start()
{
  m_enable.on();
}

void Motor::stop()
{
  m_enable.off();
}

void Motor::setRPM(float rpm)
{
 if (rpm < MIN_MOTOR_VELOCITY_RPM) rpm = MIN_MOTOR_VELOCITY_RPM;
 if (rpm > MAX_MOTOR_VELOCITY_RPM) rpm = MAX_MOTOR_VELOCITY_RPM;

  const float A = 0.0008f;
  const float B = 0.10f;

  float dutyCycle = A * rpm + B;
  m_pwm.update(dutyCycle);
}

float Motor::getMotorVelocity()
{
  return m_motorVelocitySampler->convertRawAdcValue();
}

bool Motor::isError()
{
  return m_error.isOn();
}
