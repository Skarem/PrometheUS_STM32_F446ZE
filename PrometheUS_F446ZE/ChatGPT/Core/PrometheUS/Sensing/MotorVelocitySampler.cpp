#include "MotorVelocitySampler.hpp"

MotorVelocitySampler* MotorVelocitySampler::instance = nullptr;

void MotorVelocitySampler::init(ADC_HandleTypeDef* hadc, uint32_t channel)
{
  GenericAdcChannelSampler::init(hadc, channel);
  instance = this;
}

float MotorVelocitySampler::convertRawAdcValue() const
{
  // Conversion
  // ...
  return m_rawAdcValue;
}
