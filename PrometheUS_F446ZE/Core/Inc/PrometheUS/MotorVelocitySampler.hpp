#pragma once

#include <GenericAdcChannelSampler.hpp>

class MotorVelocitySampler : public GenericAdcChannelSampler
{
public:
  using GenericAdcChannelSampler::GenericAdcChannelSampler; // Inherit constructor

  float convertRawAdcValue() const override;

private:
  // Add conversion constants here
};
