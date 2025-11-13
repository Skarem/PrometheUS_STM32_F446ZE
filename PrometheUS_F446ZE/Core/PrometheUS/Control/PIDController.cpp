#include "PIDController.hpp"

/* Inputs:
 * float: targetCurrent (A)
 * float: measuredCurrent (A)
 *
 * Output:
 * float: [0.05f - 0.95f]
 */
float PIDController::update(float setpointCurrent_A, float feedbackCurrent_A)
{
  float error = setpointCurrent_A - feedbackCurrent_A;

  if (!m_preventIntegratingWhenSaturated || (m_lastSaturationValue == 0.0f))
  {
    m_ITerm += (m_ki * error) - (m_ki * m_lastSaturationValue);
  }

  float deltaInput = feedbackCurrent_A - m_lastInput;
  float output = (m_kp * error) + m_ITerm - (m_kd * deltaInput);

  float outputBeforeSaturation = output;

  if (output > m_maximalOutput)
  {
    output = m_maximalOutput;
  }
  else if (output < m_minimalOutput)
  {
    output = m_minimalOutput;
  }
  m_lastInput = feedbackCurrent_A;
  m_lastSaturationValue = outputBeforeSaturation - output;

  return output;
}
