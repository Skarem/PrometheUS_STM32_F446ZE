#pragma once

#include "GenericAdcBurstSampler.hpp"
#include "DefineConstants.hpp"

class ClutchesTemperatureSampler : public GenericAdcBurstSampler
{
public:
  ClutchesTemperatureSampler();

  void convertAll(float (&outValues)[FINGER_COUNT]) const override;

  float testConversion(float voltage);

private:
  // Length of tables
  static constexpr uint16_t TABLE_LENGTH = 8;

  // Temperature values in descending order
  static constexpr float TEMPERATURE_TABLE_CELSIUS[TABLE_LENGTH] =
  {
      125.0f,
      100.0f,
      75.0f,
      50.0f,
      25.0f,
      0.0f,
      -25.0f,
      -40.0f
  };

#if USING_PULLDOWN_THERMISTOR
  // Resistance values in ascending order (with pull-down 51 KOhm)
  static constexpr float RESISTANCE_TABLE_OHM[TABLE_LENGTH] =
  {
      528.0f,
      956.0f,
      1852.0f,
      3838.0f,
      8333.0f,
      17650.0f,
      31693.0f,
      39517.0f
  };
#else
  // Resistance values in ascending order (without pull-down 51 KOhm)
  static constexpr float RESISTANCE_TABLE_OHM[TABLE_LENGTH] =
  {
      534.0f,
      975.0f,
      1924.0f,
      4158.0f,
      10000.0f,
      27280.0f,
      86560.0f,
      188500.0f
  };
#endif

  static constexpr float VOLTAGE_DIVIDER_RESISTANCE_OHM = 3320.0f;

  static constexpr float VOLTAGE_REFERENCE = 3.2f;

  static float VOLTAGE_TABLE_VOLT[TABLE_LENGTH];
};
