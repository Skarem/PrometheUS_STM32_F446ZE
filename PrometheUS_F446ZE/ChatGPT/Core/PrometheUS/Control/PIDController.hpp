#pragma once

#include <stdint.h>

class PIDController
{
public:
  PIDController()   = default;
  ~PIDController()  = default;

  uint16_t update(float targetCurrent, float measuredCurrent);

private:
  const float m_kp = 1.0f;
  const float m_ki = 1.0f;
  const float m_kd = 1.0f;

  const float m_minCurrent = 0.0f; // V
  const float m_maxCurrent = 0.0f; // V

  const float m_dt = 0.001f; // 1KHz

  float m_integral            = 0.0f;
  float m_prevError           = 0.0f;;
  float m_prevMeasuredCurrent = 0.0f;;

  bool m_firstUpdate = true;
};
