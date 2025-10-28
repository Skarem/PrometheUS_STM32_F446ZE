#pragma once

#include "MotorVelocitySampler.hpp"
#include "PWM.hpp"

#include "DigitalInput.hpp"
#include "DigitalOutput.hpp"

class Motor
{
public:
  Motor(MotorVelocitySampler &motorVelocitySampler);
  ~Motor() = default;

  void init();

  bool isError();
  void stop();

private:
  // ===== Input =====

  // Motor speed
  // MotorVelocitySampler class already exists
  MotorVelocitySampler m_motorVelocitySampler;

  // GPIO pin in error mode or not
  DigitalInput m_error;

  // ===== Output =====

  // PWM (Constant speed) @ 1KHz
  // Owns one TIMER CHANNEL that corresponds to the PWM Generation
  // Can be set to CONSTANT_WORKING_SPEED or 0 in case of error detected
  PWM m_pwm;

  // GPIO pin ON / OFF
  // ON at start, OFF when error.
  DigitalOutput m_enable;

  const float CONSTANT_MOTOR_SPEED = 0;
};
