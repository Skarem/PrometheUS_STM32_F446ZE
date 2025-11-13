#pragma once

#include <stdint.h>

class PIDController
{
public:
  PIDController()   = default;
  ~PIDController()  = default;

  float update(float setpointCurrent_A, float feedbackCurrent_A);

private:
  // PID Gains
  const float m_kp = 1.15f;
  const float m_ki = 0.10f;
  const float m_kd = 0.00f;

  // PID Output Limits
  const float m_minimalOutput = 0.05f;
  const float m_maximalOutput = 0.95f;

  const bool m_preventIntegratingWhenSaturated = true;

  float m_ITerm;
  float m_lastInput;
  float m_lastSaturationValue;
};
