#pragma once

#include "SystemFlags.hpp"

namespace Tests
{
  // Starts and stops PWM clutch outputs fast. Can change the duty cycle also
  void pwm();

  // TODO
  void motor();

  // Sends data at 1KHz to a real-time visualizer
  void sender();

  // ----- Samplers -----
  // Samples and converts potentiometer data at 1KHz
  void potentiometers(SystemFlags &systemFlags);

  // Samples and converts clutch temperatures data at 1KHz
  void clutchTemperatures(SystemFlags &systemFlags);
}

