#include "GenericAdcBurstSampler.hpp"

GenericAdcBurstSampler::GenericAdcBurstSampler(ADC_HandleTypeDef* hadc)
  : m_hadc(hadc)
{
  // Start hardware ADC DMA Sampling triggered by TIM2's OC2REF
  HAL_ADC_Start_DMA(m_hadc, m_rawAdcValueArray, CHANNEL_COUNT);
}
