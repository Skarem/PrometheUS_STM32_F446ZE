#pragma once

#include <GenericAdcChannelSampler.hpp>

class ClutchCurrentSampler : public GenericAdcChannelSampler
{
public:
  using GenericAdcChannelSampler::GenericAdcChannelSampler; // Inherit constructor

  float convertRawAdcValue() const override;

private:
  // Add conversion constants here
};
