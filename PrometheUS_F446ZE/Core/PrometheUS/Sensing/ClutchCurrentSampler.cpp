#include "ClutchCurrentSampler.hpp"

ClutchCurrentSampler* ClutchCurrentSampler::instances[FINGER_COUNT] = { nullptr };

void ClutchCurrentSampler::init(ADC_HandleTypeDef* hadc, uint32_t channel, uint8_t index)
{
  GenericAdcChannelSampler::init(hadc, channel, index);
  if (index < FINGER_COUNT)
  {
    instances[index] = this;
  }
}

float ClutchCurrentSampler::convertRawAdcValue() const
{
  float voltage = AdcUtils::getAdcVoltage(m_rawAdcValue);
  float current = (voltage - OFFSET_ADC_VOLT) * GAIN_AMPERE_PER_ADC_VOLT;
  return voltage;
}

void ClutchCurrentSampler::getInjectedValue(uint16_t value)
{
  m_rawAdcValue = value;
}

float ClutchCurrentSampler::testConversion(float voltage)
{
  float current = (voltage - OFFSET_ADC_VOLT) * GAIN_AMPERE_PER_ADC_VOLT;
  return current;
}
