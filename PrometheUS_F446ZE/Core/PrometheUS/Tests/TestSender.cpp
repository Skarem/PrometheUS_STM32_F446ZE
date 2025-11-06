#include "TestsNamespace.hpp"

#include "TelemetrySender.hpp"
#include <cmath>

static inline float mapSine(float t, float freq, float phase, float minVal, float maxVal)
{
  float mid = 0.5f * (maxVal + minVal);
  float amp = 0.5f * (maxVal - minVal);
  return mid + amp * sinf(2.0f * static_cast<float>(M_PI) * freq * t + phase);
}

/**
 * @file  TestSender.cpp
 * @brief Unit test for TelemetrySender communication over UART3
 *
 * @details
 * This test validates the data formatting and transmission of the TelemetrySender class.
 * Generates synthetic sine and cosine waveforms for all telemetry channels
 * (potentiometers, encoders, clutch currents, and motor velocity).
 * Transmits them at a fixed rate (~50Hz) over UART3.
 *
 * Transmits data over UART3 with the USB cable
 *
 * USART3_TX -> PD8
 * USART3_RX -> PD9 (not needed)
 */
void Tests::sender()
{
  TelemetrySender sender;
  sender.init(&huart3);

  const float FREQUENCY     = 50.0f; // Hz
  const uint32_t PERIOD_MS  = static_cast<uint32_t>(1000.0f / FREQUENCY);
  const float SIGNAL_FREQ   = 0.25f; // Hz of sine waves

  uint32_t nextTime = HAL_GetTick();
  const float TWO_PI = 2.0f * static_cast<float>(M_PI);

  while (true)
  {
    uint32_t now = HAL_GetTick();
    if ((int32_t)(now - nextTime) >= 0)
    {
      nextTime += PERIOD_MS;

      float t = now / 1000.0f;

      float pots[FINGER_COUNT];
      float temp[FINGER_COUNT];
      float curr[FINGER_COUNT];
      float motorVelocity;

      bool    inError     = false;
      uint8_t errorSource = static_cast<uint8_t>(ErrorSource::NONE);

      for (size_t i = 0; i < FINGER_COUNT; ++i)
      {
        float phase = (TWO_PI / static_cast<float>(FINGER_COUNT)) * i;

        pots[i] = mapSine(t, SIGNAL_FREQ, phase, MIN_POTENTIOMETER_POSITION_DEG, MAX_POTENTIOMETER_POSITION_DEG);
        temp[i] = mapSine(t, SIGNAL_FREQ, phase + M_PI / 2.0f, MIN_CLUTCH_TEMPERATURE_CELSIUS, MAX_CLUTCH_TEMPERATURE_CELSIUS);
        curr[i] = mapSine(t, SIGNAL_FREQ, phase + M_PI / 3.0f, MIN_CLUTCH_CURRENT_AMPERE, MAX_CLUTCH_CURRENT_AMPERE);
      }
      motorVelocity = mapSine(t, SIGNAL_FREQ, 0.0f, MIN_MOTOR_VELOCITY_RPM, MAX_MOTOR_VELOCITY_RPM);

      sender.send(now, pots, temp, curr, motorVelocity, inError, errorSource);

      HAL_Delay(1);
    }
  }
}
