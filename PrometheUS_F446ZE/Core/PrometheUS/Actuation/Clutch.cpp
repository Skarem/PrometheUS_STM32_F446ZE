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
  return m_clutchCurrentSampler.convertRawAdcValue();
}

void Clutch::updateCommand()
{
  m_pwmComplementary.update(m_dutyCycle);
}

void Clutch::stop()
{
  m_pwmComplementary.stop();
}
