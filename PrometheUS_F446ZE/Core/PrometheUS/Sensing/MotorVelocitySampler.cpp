#include "MotorVelocitySampler.hpp"
#include "DefineConstants.hpp"

MotorVelocitySampler* MotorVelocitySampler::instance = nullptr;

void MotorVelocitySampler::init(ADC_HandleTypeDef* hadc, uint32_t channel)
{
  GenericAdcChannelSampler::init(hadc, channel, NOT_FINGER_INDEX);
  instance = this;
}

float MotorVelocitySampler::convertRawAdcValue() const
{
  // Conversion
  // ...
  return m_rawAdcValue;
}
