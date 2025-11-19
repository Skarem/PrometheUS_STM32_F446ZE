#pragma once

#include "DefineConstants.hpp"
#include <algorithm>

struct ErrorFlags
{
  bool clutchTemperatures[FINGER_COUNT] = { false };
  bool clutchCurrents[FINGER_COUNT]     = { false };
  bool motor                            = false;

  bool anyError() const
  {
    for (size_t i = 0; i < FINGER_COUNT; ++i)
    {
      if (clutchTemperatures[i] || clutchCurrents[i])
      {
        return true;
      }
    }
    return motor;
  }

  void clearFlags()
  {
    for (size_t i = 0; i < FINGER_COUNT; ++i)
    {
      clutchTemperatures[i] = false;
      clutchCurrents[i]     = false;
    }
    motor = false;
  }
};
