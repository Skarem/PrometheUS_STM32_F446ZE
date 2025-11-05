#pragma once

#include "GenericAdcBurstSampler.hpp"

class PotentiometersSampler : public GenericAdcBurstSampler
{
public:
  using GenericAdcBurstSampler::GenericAdcBurstSampler; // Inherit constructor

  void convertAll(float (&outValues)[FINGER_COUNT]) const override;

private:
  static constexpr float ADC_MAX   = 4095.0f;
  static constexpr float ANGLE_MAX = 360.0f;
};
