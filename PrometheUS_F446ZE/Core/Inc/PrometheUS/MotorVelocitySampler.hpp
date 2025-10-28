#pragma once

#include "GenericAdcChannelSampler.hpp"

class MotorVelocitySampler : public GenericAdcChannelSampler
{
public:
  static MotorVelocitySampler* instance;

  void init(ADC_HandleTypeDef* hadc, uint32_t channel);

  float convertRawAdcValue() const override;

private:
  // Add conversion constants here
};
