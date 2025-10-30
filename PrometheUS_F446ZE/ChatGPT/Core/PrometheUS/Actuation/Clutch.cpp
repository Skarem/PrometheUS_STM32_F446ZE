#include "Clutch.hpp"

#include "main.h"

Clutch::Clutch(ClutchCurrentSampler &clutchCurrentSampler)
  : m_clutchCurrentSampler(clutchCurrentSampler)
{

}

void Clutch::init(uint32_t currentChannel, uint8_t currentIndex, uint32_t pwmChannel)
{
  m_clutchCurrentSampler.init(&hadc3, currentChannel, currentIndex);
  m_pwmComplementary.init(&htim1, pwmChannel);
}

void Clutch::calculateCurrentPID(float targetCurrent, float measuredCurrent)
{
  m_dutyCycle = m_pidController.update(targetCurrent, measuredCurrent);
}

float Clutch::getMeasuredCurrent()
{
  m_lastMeasuredClutchCurrent = m_clutchCurrentSampler.convertRawAdcValue();
  return m_lastMeasuredClutchCurrent;
}

bool Clutch::isInError()
{
  if (m_lastMeasuredClutchCurrent < MIN_CLUTCH_CURRENT ||
      m_lastMeasuredClutchCurrent > MAX_CLUTCH_CURRENT)
  {
    return true;
  }
  return false;
}

void Clutch::updateCommand()
{
  m_dutyCycle = 0.5f;
  m_pwmComplementary.update(m_dutyCycle);
}

void Clutch::start()
{
  m_pwmComplementary.start();
}

void Clutch::stop()
{
  m_pwmComplementary.stop();
}
