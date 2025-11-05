#pragma once

// ---------- System-wide ----------
static constexpr uint8_t FINGER_COUNT = 3;

static constexpr uint8_t FINGER_1_INDEX = 0;
static constexpr uint8_t FINGER_2_INDEX = 1;
static constexpr uint8_t FINGER_3_INDEX = 2;

// ---------- Telemetry sender ----------
static constexpr uint16_t TELEMETRY_HEADER = 0xABCD;

// ---------- Samplers ----------
namespace AdcUtils
{
  static constexpr float ADC_MINIMUM_COUNT_VALUE = 0.0f;
  static constexpr float ADC_MAXIMAL_COUNT_VALUE = 4095.0f;

  static constexpr float ADC_REFERENCE_VOLTAGE = 3.3f;
  static constexpr float ADC_TO_VOLT = ADC_REFERENCE_VOLTAGE / ADC_MAXIMAL_COUNT_VALUE;

  inline float getAdcVoltage(uint16_t adcResult)
  {
    if (adcResult > ADC_MAXIMAL_COUNT_VALUE)
    {
      adcResult = ADC_MAXIMAL_COUNT_VALUE;
    }
    return adcResult * ADC_TO_VOLT;
  }
};
