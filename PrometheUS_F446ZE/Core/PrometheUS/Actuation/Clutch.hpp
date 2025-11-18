#pragma once

#include "PWMComplementary.hpp"
#include "ClutchCurrentSampler.hpp"
#include "PIDController.hpp"

class Clutch
{
public:
  Clutch(ClutchCurrentSampler* clutchCurrentSampler);
  ~Clutch() = default;

  void init(uint32_t currentChannel, uint8_t currentIndex, uint32_t pwmChannel, GPIO_TypeDef* portPWMn, uint16_t pinPWMn);

  float calculateCurrentPID(float targetCurrent, float measuredCurrent);

  float getMeasuredCurrent();
  bool isInError();

  void updateCommand();

  void start();
  void stop();

private:

  // ===== Input =====

  // Clutch current
  ClutchCurrentSampler* m_clutchCurrentSampler;
  float                 m_lastMeasuredClutchCurrent = 0.0f;

  const float MIN_CLUTCH_CURRENT = 0.0f;
  const float MAX_CLUTCH_CURRENT = 4.0f;

  // ===== Outputs =====

  // PWMs
  PWMComplementary  m_pwmComplementary;
  float             m_PIDResult = 0;

  // PID current controller
  PIDController m_pidController;
};
