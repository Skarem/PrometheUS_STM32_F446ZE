#include "Clutch.hpp"

#include "main.h"

Clutch::Clutch(ClutchCurrentSampler* clutchCurrentSampler)
  : m_clutchCurrentSampler(clutchCurrentSampler)
{

}

void Clutch::init(uint32_t currentChannel, uint8_t currentIndex, uint32_t pwmChannel, GPIO_TypeDef* portPWMn, uint16_t pin_PWMn)
{
  m_clutchCurrentSampler->init(&hadc3, currentChannel, currentIndex);
  m_pwmComplementary.init(&htim1, pwmChannel, portPWMn, pin_PWMn);
}

float Clutch::calculateCurrentPID(float targetCurrent, float measuredCurrent)
{
  m_PIDResult = m_pidController.update(targetCurrent, measuredCurrent);
  return m_PIDResult;
}

float Clutch::getMeasuredCurrent()
{
  m_lastMeasuredClutchCurrent = m_clutchCurrentSampler->convertRawAdcValue();
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

void Clutch::setCommand(float dutyCycle)
{
  m_PIDResult = dutyCycle;
}

void Clutch::updateCommand()
{
  m_pwmComplementary.update(m_PIDResult);
}

void Clutch::start()
{
  m_pwmComplementary.start();
}

void Clutch::stop()
{
  m_pwmComplementary.stop();
}
