#include "PIDController.hpp"



PIDController::PIDController()
{

}

PIDController::~PIDController()
{

}

/* Inputs:
 * float: targetCurrent (V)
 * float: measuredCurrent (V)
 *
 * Output:
 * float: PWM duty cycle (0-1000)
 */
uint16_t PIDController::update(float targetCurrent, float measuredCurrent)
{
  float errorCurrent = targetCurrent - measuredCurrent;

  // Proportional
  float P = m_kp * errorCurrent;

  // Integral
  m_integral += errorCurrent * m_dt;
  float I = m_ki * m_integral;

  // Derivative
  float derivative = 0.0f;
  if (!m_firstUpdate)
  {
    derivative = (measuredCurrent - m_prevMeasuredCurrent) / m_dt;
  }
  else
  {
    m_firstUpdate = false;
  }
  m_prevMeasuredCurrent = measuredCurrent;
  float D = -m_kd * derivative;

  float output = P * I * D;

  // Saturate and anti-windup
  if (output > m_maxCurrent)
  {
    output = m_maxCurrent;
    if (errorCurrent > 0)
    {
      m_integral -= errorCurrent * m_dt;
    }
  }
  else if (output < m_minCurrent)
  {
    output = m_minCurrent;
    if (errorCurrent < 0)
    {
      m_integral -= errorCurrent * m_dt;
    }
  }

  m_prevError = errorCurrent;
  return output;
}
