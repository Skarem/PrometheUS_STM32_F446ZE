#include "PotentiometersSampler.hpp"

void PotentiometersSampler::convertAll(float (&outValues)[FINGER_COUNT]) const
{
  for (size_t i = 0; i < FINGER_COUNT; ++i)
  {
    outValues[i] = m_rawAdcValueArray[i];
  }
}
