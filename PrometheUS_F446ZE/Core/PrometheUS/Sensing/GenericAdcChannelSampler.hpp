#pragma once

#include "stm32f4xx_hal.h"

class GenericAdcChannelSampler
{
public:
  GenericAdcChannelSampler()          = default;
  virtual ~GenericAdcChannelSampler() = default;

  // Initialize with ADC handle and channel number
  void init(ADC_HandleTypeDef* hadc, uint32_t adcChannel);

  // Start sampling this channel
  void startSamplingRawAdcValue();

  [[nodiscard]] uint32_t getRawValue() const { return m_rawAdcValue; }

  // Child class implements its own conversion formula
  [[nodiscard]] virtual float convertRawAdcValue() const = 0;

protected:
  uint16_t m_rawAdcValue = 0;

private:
  ADC_HandleTypeDef* m_hadc = nullptr;
  ADC_ChannelConfTypeDef m_adcConfiguration = {};

  static constexpr uint32_t ADC3_CONFIGURATION_RANK          = 1;
  static constexpr uint32_t ADC3_CONFIGURATION_SAMPLING_TIME = ADC_SAMPLETIME_3CYCLES;
  static constexpr uint32_t ADC3_CONFIGURATION_OFFSET        = 0;

  static ADC_ChannelConfTypeDef CreateAdcConfiguration(uint32_t channel);
};
