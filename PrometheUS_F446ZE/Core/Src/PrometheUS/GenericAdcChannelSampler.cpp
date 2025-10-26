#include "GenericAdcChannelSampler.hpp"

GenericAdcChannelSampler::GenericAdcChannelSampler(ADC_HandleTypeDef* hadc, uint32_t adcChannel)
  : m_hadc(hadc),
    m_adcConfiguration(CreateAdcConfiguration(adcChannel))
{

}

void GenericAdcChannelSampler::startSamplingRawAdcValue()
{
  // Change channel
  HAL_ADC_ConfigChannel(m_hadc, &m_adcConfiguration);
  // Start sampling
  HAL_ADC_Start_DMA(m_hadc, &m_rawAdcValue, 1);
}
