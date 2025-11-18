#pragma once

// ---------- System-wide ----------
static constexpr uint8_t FINGER_COUNT = 3;

static constexpr uint8_t FINGER_1_INDEX   = 0;
static constexpr uint8_t FINGER_2_INDEX   = 1;
static constexpr uint8_t FINGER_3_INDEX   = 2;
static constexpr uint8_t NOT_FINGER_INDEX = 100;

// ---------- Telemetry sender ----------
static constexpr uint16_t TELEMETRY_HEADER = 0xABCD;

// ---------- Samplers ----------

#define USING_PULLDOWN_THERMISTOR false

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

constexpr float MIN_POTENTIOMETER_POSITION_DEG = 0.0f;
constexpr float MAX_POTENTIOMETER_POSITION_DEG = 360.0f;

constexpr float MIN_CLUTCH_TEMPERATURE_CELSIUS = 0.0f;
constexpr float MAX_CLUTCH_TEMPERATURE_CELSIUS = 80.0f;

constexpr float MIN_CLUTCH_CURRENT_AMPERE = 0.0f;
constexpr float MAX_CLUTCH_CURRENT_AMPERE = 4.0f;

constexpr float MIN_MOTOR_VELOCITY_RPM = 40.0f;
constexpr float MAX_MOTOR_VELOCITY_RPM = 1000.0f;
