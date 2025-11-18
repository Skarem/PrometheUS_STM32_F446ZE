#pragma once

#include "PWM.hpp"
#include "MotorVelocitySampler.hpp"

#include "DigitalInput.hpp"
#include "DigitalOutput.hpp"

class Motor
{
public:
  Motor(MotorVelocitySampler* motorVelocitySampler);
  ~Motor() = default;

  void init();

  void start();
  void stop();

  void setRPM(float rpm);

  float getMotorVelocity();

  bool isError();

private:
  // ===== Input =====

  // Motor speed
  MotorVelocitySampler* m_motorVelocitySampler;

  // GPIO pin in error mode or not
  DigitalInput m_error;

  // ===== Output =====

  // PWM (Constant speed) @ 10 KHz
  PWM m_pwm;

  // GPIO pin ON / OFF
  DigitalOutput m_enable;

  const float CONSTANT_MOTOR_SPEED = 0;
};
