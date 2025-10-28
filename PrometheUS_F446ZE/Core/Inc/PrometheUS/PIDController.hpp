#pragma once

#include <stdint.h>

class PIDController
{
public:
  PIDController();
  ~PIDController();

  uint16_t update(float targetCurrent, float measuredCurrent);

private:
  const float m_kp = 1;
  const float m_ki = 1;
  const float m_kd = 1;

  const float m_minCurrent = 0; // V
  const float m_maxCurrent = 0; // V

  const float m_dt = 0.001f; // 1KHz

  float m_integral;
  float m_prevError;
  float m_prevMeasuredCurrent;

  bool m_firstUpdate = true;
};
