#pragma once

#include "DefineConstants.hpp"
#include <algorithm>

struct ErrorFlags
{
  bool clutchTemperatures[FINGER_COUNT] = { false };
  bool motor                            = false;

  bool anyError() const
  {
    bool error = clutchTemperatures[FINGER_1_INDEX] ||
                 clutchTemperatures[FINGER_2_INDEX] ||
                 clutchTemperatures[FINGER_3_INDEX] ||
                 motor;
    return error;
  }

  void clearFlags()
  {
    clutchTemperatures[FINGER_1_INDEX] = false;
    clutchTemperatures[FINGER_2_INDEX] = false;
    clutchTemperatures[FINGER_3_INDEX] = false;
    motor = false;
  }
};
