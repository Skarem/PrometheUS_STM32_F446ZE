#include "ClutchesTemperatureSampler.hpp"

#include "Lookup.hpp"
#include "DefineConstants.hpp"

// Static member definitions
constexpr float ClutchesTemperatureSampler::TEMPERATURE_TABLE_CELSIUS[TABLE_LENGTH];
constexpr float ClutchesTemperatureSampler::RESISTANCE_TABLE_OHM[TABLE_LENGTH];
float ClutchesTemperatureSampler::VOLTAGE_TABLE_VOLT[TABLE_LENGTH];

ClutchesTemperatureSampler::ClutchesTemperatureSampler()
  : GenericAdcBurstSampler()
{
  for (size_t i = 0; i < TABLE_LENGTH; i++)
  {
    VOLTAGE_TABLE_VOLT[i] = VOLTAGE_REFERENCE * (RESISTANCE_TABLE_OHM[i] / (VOLTAGE_DIVIDER_RESISTANCE_OHM + RESISTANCE_TABLE_OHM[i]));
  }
}

void ClutchesTemperatureSampler::convertAll(float (&outValues)[FINGER_COUNT]) const
{
  for (size_t i = 0; i < FINGER_COUNT; ++i)
  {
    float voltage = AdcUtils::getAdcVoltage(m_rawAdcValueArray[i]);

    float temperature = lookup1d(voltage, VOLTAGE_TABLE_VOLT, TEMPERATURE_TABLE_CELSIUS, TABLE_LENGTH);

    outValues[i] = temperature;
  }
}

float ClutchesTemperatureSampler::testConversion(float voltage)
{
  float temperature = lookup1d(voltage, VOLTAGE_TABLE_VOLT, TEMPERATURE_TABLE_CELSIUS, TABLE_LENGTH);
  return temperature;
}
