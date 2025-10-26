#pragma once

#include "main.h"

class GenericAdcBurstSampler
{
protected:
  static constexpr uint8_t CHANNEL_COUNT = 3;

public:
  explicit GenericAdcBurstSampler(ADC_HandleTypeDef* hadc);
  virtual ~GenericAdcBurstSampler() = default;

  virtual void convertAll(float (&outValues)[CHANNEL_COUNT]) const = 0;

protected:
  uint32_t m_rawAdcValueArray[CHANNEL_COUNT];

private:
  ADC_HandleTypeDef* const m_hadc;
};
