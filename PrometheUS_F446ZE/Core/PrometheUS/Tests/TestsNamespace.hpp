#pragma once

#include "SystemFlags.hpp"
#include "ClutchCurrentSampler.hpp"

namespace Tests
{
  // Starts and stops PWM clutch outputs fast, sweeps duty-cycle
  void pwm();

  void motor();

  // Sends data at 1KHz to a real-time visualizer
  void sender();
  // Sends error messages
  void senderErrors();

  // ----- Samplers -----
  // Samples and converts potentiometer data at 1KHz
  void potentiometers(SystemFlags &systemFlags);

  // Samples and converts clutch temperatures data at 1KHz
  void clutchTemperatures(SystemFlags &systemFlags);

  // Samples and converts clutch currents data at 1KHz
  void clutchCurrents(SystemFlags &systemFlags,
      ClutchCurrentSampler &clutchCurrentSampler1,
      ClutchCurrentSampler &clutchCurrentSampler2,
      ClutchCurrentSampler &clutchCurrentSampler3);

  void button();

  void LEDs();
}

