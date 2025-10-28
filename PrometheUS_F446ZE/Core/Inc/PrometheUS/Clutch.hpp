#pragma once

#include "ClutchCurrentSampler.hpp"
#include "PWMComplementary.hpp"
#include "PIDController.hpp"

class Clutch
{
public:
  Clutch(ClutchCurrentSampler &clutchCurrentSampler);
  ~Clutch() = default;

  void init(uint32_t currentChannel, uint8_t currentIndex, uint32_t pwmChannel);

  void calculateCurrentPID(float targetCurrent, float measuredCurrent);

  float getMeasuredCurrent();

  void updateCommand();
  void stop();

private:

  // ===== Input =====

  // Clutch temperature
  // Gets fed one temperature from the clutches temperature sampler (ADC Burst Mode)
  // float m_clutchTemperature;

  // Clutch current
  ClutchCurrentSampler  m_clutchCurrentSampler;

  // ===== Output =====

  // PWMs
  // Owns and starts one TIMER CHANNEL for the PWM Generation (PWM and PWMn)
  PWMComplementary  m_pwmComplementary;
  uint16_t          m_dutyCycle = 0;

  // PID current controller
  PIDController m_pidController;
};
