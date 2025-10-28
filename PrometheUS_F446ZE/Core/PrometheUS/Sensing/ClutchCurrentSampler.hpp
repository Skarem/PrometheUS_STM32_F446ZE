#pragma once

#include "GenericAdcChannelSampler.hpp"
#include "DefineConstants.hpp"

class ClutchCurrentSampler : public GenericAdcChannelSampler
{
public:
  static ClutchCurrentSampler* instances[FINGER_COUNT];

  void init(ADC_HandleTypeDef* hadc, uint32_t channel, uint8_t index);

  float convertRawAdcValue() const override;

private:
  // Add conversion constants here
};
