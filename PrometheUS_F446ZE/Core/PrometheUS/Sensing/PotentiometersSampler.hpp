#pragma once

#include "GenericAdcBurstSampler.hpp"

class PotentiometersSampler : public GenericAdcBurstSampler
{
public:
  using GenericAdcBurstSampler::GenericAdcBurstSampler; // Inherit constructor

  void convertAll(float (&outValues)[FINGER_COUNT]) const override;

private:
};
