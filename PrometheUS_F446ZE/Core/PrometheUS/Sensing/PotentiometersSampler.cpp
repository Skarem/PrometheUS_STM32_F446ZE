#include "PotentiometersSampler.hpp"

void PotentiometersSampler::convertAll(float (&outValues)[FINGER_COUNT]) const
{
  for (size_t i = 0; i < FINGER_COUNT; ++i)
  {
    outValues[i] = (static_cast<float>(m_rawAdcValueArray[i]) / ADC_MAX) * ANGLE_MAX;
  }
}
