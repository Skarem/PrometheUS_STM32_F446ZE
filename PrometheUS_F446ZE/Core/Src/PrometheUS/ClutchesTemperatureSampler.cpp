#include "ClutchesTemperatureSampler.hpp"

void ClutchesTemperatureSampler::convertAll(float (&outValues)[FINGER_COUNT]) const
{
  for (size_t i = 0; i < FINGER_COUNT; ++i)
  {
    outValues[i] = m_rawAdcValueArray[i];
  }
}
