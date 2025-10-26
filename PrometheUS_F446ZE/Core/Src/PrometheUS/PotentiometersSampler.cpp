#include "PotentiometersSampler.hpp"

void PotentiometersSampler::convertAll(float (&outValues)[CHANNEL_COUNT]) const
{
  for (size_t i = 0; i < CHANNEL_COUNT; ++i)
  {
    outValues[i] = m_rawAdcValueArray[i];
  }
}
