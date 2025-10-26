#pragma once

#include "GenericAdcBurstSampler.hpp"

class PotentiometersSampler : public GenericAdcBurstSampler
{
public:
  using GenericAdcBurstSampler::GenericAdcBurstSampler; // Inherit constructor

  void convertAll(float (&outValues)[CHANNEL_COUNT]) const override;

private:
  // Add conversion constants here
};
