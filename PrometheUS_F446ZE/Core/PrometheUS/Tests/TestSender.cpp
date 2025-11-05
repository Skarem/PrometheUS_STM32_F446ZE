#include "TestsNamespace.hpp"

#include "TelemetrySender.hpp"
#include <cmath>

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
      float encs[FINGER_COUNT];
      float curr[FINGER_COUNT];
      float motorVelocity;

      for (size_t i = 0; i < FINGER_COUNT; ++i)
      {
        float phase = (TWO_PI / static_cast<float>(FINGER_COUNT)) * i;
        pots[i] = 10.0f + 2.0f * sinf(TWO_PI * SIGNAL_FREQ * t + phase);
        encs[i] = 15.0f + 2.0f * cosf(TWO_PI * SIGNAL_FREQ * t + phase);
        curr[i] = 20.0f + 2.0f * sinf(TWO_PI * SIGNAL_FREQ * t + phase + M_PI/3.0f);
      }
      motorVelocity = 3250.0f + 500.0f * sinf(TWO_PI * SIGNAL_FREQ * t);

      sender.send(now, pots, encs, curr, motorVelocity);

      HAL_Delay(1);
    }
  }
}
