#pragma once

#include "main.h"

class GenericAdcChannelSampler
{
public:
  explicit GenericAdcChannelSampler(ADC_HandleTypeDef* hadc, uint32_t adcChannel);
  virtual ~GenericAdcChannelSampler() = default;

  void startSamplingRawAdcValue();
  [[nodiscard]] virtual float convertRawAdcValue() const = 0;

protected:
  uint32_t m_rawAdcValue = 0;

private:
  ADC_HandleTypeDef* const m_hadc;
  ADC_ChannelConfTypeDef m_adcConfiguration;

  static constexpr uint32_t ADC3_CONFIGURATION_RANK          = 1;
  static constexpr uint32_t ADC3_CONFIGURATION_SAMPLING_TIME = ADC_SAMPLETIME_3CYCLES;
  static constexpr uint32_t ADC3_CONFIGURATION_OFFSET        = 0;

  static constexpr ADC_ChannelConfTypeDef CreateAdcConfiguration(uint32_t channel)
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
};

