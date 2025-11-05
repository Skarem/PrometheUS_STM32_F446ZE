#pragma once

#include "main.h"

#include "DefineConstants.hpp"

class GenericAdcBurstSampler
{
public:
  GenericAdcBurstSampler()          = default;
  virtual ~GenericAdcBurstSampler() = default;

  void init(ADC_HandleTypeDef* hadc);

  virtual void convertAll(float (&outValues)[FINGER_COUNT]) const = 0;

protected:
  uint16_t m_rawAdcValueArray[FINGER_COUNT] = { 0 };

private:
  ADC_HandleTypeDef* m_hadc = nullptr;
};
