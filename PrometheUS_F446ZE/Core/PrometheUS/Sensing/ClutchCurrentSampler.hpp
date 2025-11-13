#pragma once

#include "GenericAdcChannelSampler.hpp"
#include "DefineConstants.hpp"

class ClutchCurrentSampler : public GenericAdcChannelSampler
{
public:
  static ClutchCurrentSampler* instances[FINGER_COUNT];

  void init(ADC_HandleTypeDef* hadc, uint32_t channel, uint8_t index);

  float convertRawAdcValue() const override;

  static float testConversion(float voltage);

private:
  static constexpr float OFFSET_ADC_VOLT          = 0.171f;
  static constexpr float GAIN_AMPERE_PER_ADC_VOLT = 12.22249f;
};
