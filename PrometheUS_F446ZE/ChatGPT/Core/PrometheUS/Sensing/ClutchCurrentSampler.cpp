#include "ClutchCurrentSampler.hpp"

ClutchCurrentSampler* ClutchCurrentSampler::instances[FINGER_COUNT] = { nullptr };

void ClutchCurrentSampler::init(ADC_HandleTypeDef* hadc, uint32_t channel, uint8_t index)
{
  GenericAdcChannelSampler::init(hadc, channel);
  if (index < FINGER_COUNT)
  {
    instances[index] = this;
  }
}

float ClutchCurrentSampler::convertRawAdcValue() const
{
  // Conversion
  // ...
  return m_rawAdcValue;
}
