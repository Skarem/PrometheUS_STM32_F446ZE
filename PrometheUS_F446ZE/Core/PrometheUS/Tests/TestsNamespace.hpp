#pragma once

#include "SystemFlags.hpp"
#include "ClutchCurrentSampler.hpp"
#include "MotorVelocitySampler.hpp"

namespace Tests
{
  // Starts and stops PWM clutch outputs fast, sweeps duty-cycle
  void pwm();

  void motor(SystemFlags* systemFlags, MotorVelocitySampler* motorVelocitySampler);

  // Sends data at 1KHz to a real-time visualizer
  void sender();
  // Sends error messages
  void senderErrors();

  // ----- Samplers -----
  // Samples and converts potentiometer data at 1KHz
  void potentiometers(SystemFlags* systemFlags);

  // Samples and converts clutch temperatures data at 1KHz
  void clutchTemperatures(SystemFlags* systemFlags);

  // Samples and converts clutch currents data at 1KHz
  void clutchCurrents(SystemFlags* systemFlags,
      ClutchCurrentSampler* clutchCurrentSampler1,
      ClutchCurrentSampler* clutchCurrentSampler2,
      ClutchCurrentSampler* clutchCurrentSampler3);

  // Test conversion functions
  void adcToVoltConversion();
  void adcToCurrConversion();
  void adcToTempConversion();

  void button();
  void buttonDebug();

  void LEDs();
  void LEDsDebug();

  void stateMachines(SystemFlags* systemFlags, ClutchCurrentSampler* clutch1, ClutchCurrentSampler* clutch2, ClutchCurrentSampler* clutch3, MotorVelocitySampler* motor);

  void PIDClutch(SystemFlags* systemFlags, ClutchCurrentSampler* currentSampler1, ClutchCurrentSampler* currentSampler2, ClutchCurrentSampler* currentSampler3);
}

