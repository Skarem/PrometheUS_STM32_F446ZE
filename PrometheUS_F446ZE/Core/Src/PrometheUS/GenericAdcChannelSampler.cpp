#include "GenericAdcChannelSampler.hpp"

void GenericAdcChannelSampler::init(ADC_HandleTypeDef* hadc, uint32_t adcChannel)
{
  m_hadc = hadc;
  m_adcConfiguration = CreateAdcConfiguration(adcChannel);
}

void GenericAdcChannelSampler::startSamplingRawAdcValue()
{
  if (!m_hadc) return;

  // Change channel
  HAL_ADC_ConfigChannel(m_hadc, &m_adcConfiguration);
  // Start sampling
  HAL_ADC_Start_DMA(m_hadc, &m_rawAdcValue, 1);
}

ADC_ChannelConfTypeDef GenericAdcChannelSampler::CreateAdcConfiguration(uint32_t channel)
{
  ADC_ChannelConfTypeDef configuration =
  {
    .Channel      = channel,
    .Rank         = ADC3_CONFIGURATION_RANK,
    .SamplingTime = ADC3_CONFIGURATION_SAMPLING_TIME,
    .Offset       = ADC3_CONFIGURATION_OFFSET
  };

  return configuration;
}
