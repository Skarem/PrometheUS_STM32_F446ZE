#include "PotentiometersSampler.hpp"

#include "DefineConstants.hpp"

void PotentiometersSampler::convertAll(float (&outValues)[FINGER_COUNT]) const
{
  for (size_t i = 0; i < FINGER_COUNT; ++i)
  {
    outValues[i] = (static_cast<float>(m_rawAdcValueArray[i]) / AdcUtils::ADC_MAXIMAL_COUNT_VALUE) * POTENTIOMETER_ANGLE_MAX;
  }
}
