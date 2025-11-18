#include "GenericAdcChannelSampler.hpp"
#include "main.h"
#include "DefineConstants.hpp"
#include <cstdio>

void GenericAdcChannelSampler::init(ADC_HandleTypeDef* hadc, uint32_t adcChannel, uint8_t index)
{
  m_hadc = hadc;
  m_adcConfiguration = CreateAdcConfiguration(adcChannel);
  m_index = index;
  if (m_index == FINGER_2_INDEX)
  {
    __HAL_ADC_ENABLE_IT(&hadc2, ADC_IT_JEOC);
  }
}

void GenericAdcChannelSampler::startSamplingRawAdcValue()
{
  if (m_index == FINGER_2_INDEX)
  {
    HAL_ADCEx_InjectedStart(&hadc2);
  }
  else
  {
    // Change channel
    HAL_ADC_ConfigChannel(m_hadc, &m_adcConfiguration);
    // Start sampling
    HAL_ADC_Start_DMA(m_hadc, reinterpret_cast<uint32_t*>(&m_rawAdcValue), 1);
  }
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
