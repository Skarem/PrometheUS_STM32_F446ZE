#include <TestsNamespace.hpp>
#include "main.h"

#include "PWM.hpp"
#include "PWMComplementary.hpp"

void Tests::pwm()
{
  PWM pwmMotor;
  pwmMotor.init(&htim4, TIM_CHANNEL_1);
  pwmMotor.start();
  const float MOTOR_DUTY_CYCLE = 0.25f;
  pwmMotor.update(MOTOR_DUTY_CYCLE);

  PWMComplementary pwmClutch1;
  PWMComplementary pwmClutch2;
  PWMComplementary pwmClutch3;

  pwmClutch1.init(&htim1, TIM_CHANNEL_1);
  pwmClutch2.init(&htim1, TIM_CHANNEL_2);
  pwmClutch3.init(&htim1, TIM_CHANNEL_3);

  const float DUTY_CYCLES[3] = { 0.50f };
  const size_t NUM_DUTY = sizeof(DUTY_CYCLES) / sizeof(DUTY_CYCLES[0]);

  const uint32_t STEP_TIME_MS = 1;
  const uint32_t OFF_TIME_MS  = 10;

  while (true)
  {
    pwmClutch1.start();
    pwmClutch2.start();
    pwmClutch3.start();

    for (size_t i = 0; i < NUM_DUTY; ++i)
    {
      const float dutyCycle = DUTY_CYCLES[i];

      pwmClutch1.update(dutyCycle);
      pwmClutch2.update(dutyCycle);
      pwmClutch3.update(dutyCycle);

      HAL_Delay(STEP_TIME_MS);
    }

    pwmClutch1.stop();
    pwmClutch2.stop();
    pwmClutch3.stop();

    HAL_Delay(OFF_TIME_MS);
  }
}

#include "TelemetrySender.hpp"
#include <cmath>

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

#include "PotentiometersSampler.hpp"
#include "PrometheUS.hpp"
#include <cstdio>

void Tests::potentiometers(SystemFlags &systemFlags)
{
  PotentiometersSampler potentiometers;
  potentiometers.init(&hadc1);

  PrometheUS_Gripper::startTimers();
  // TIM2 and TIM3 are synced to TIM1, so :
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  char txBuf[64];
  float potValues[3];

  while (true)
  {
    // Wait for ADC1 conversion completion flag
    uint8_t flag = systemFlags.adcDoneMask.exchange(0, std::memory_order_relaxed);
    if (flag & SystemFlags::POTS)
    {
      // Convert raw ADC values to floats
      potentiometers.convertAll(potValues);

      // Format into human-readable ASCII
      int len = snprintf(txBuf, sizeof(txBuf), "%.4f %.4f %.4f\r\n",
          potValues[0], potValues[1], potValues[2]);

      // Transmit
      HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
    }
  }
}

#include ""

void Tests::clutchTemperatures(SystemFlags &systemFlags)
{
  PotentiometersSampler potentiometers;
  potentiometers.init(&hadc1);

  PrometheUS_Gripper::startTimers();
  // TIM2 and TIM3 are synced to TIM1, so :
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  char txBuf[64];
  float potValues[3];

  while (true)
  {
    // Wait for ADC1 conversion completion flag
    uint8_t flag = systemFlags.adcDoneMask.exchange(0, std::memory_order_relaxed);
    if (flag & SystemFlags::POTS)
    {
      // Convert raw ADC values to floats
      potentiometers.convertAll(potValues);

      // Format into human-readable ASCII
      int len = snprintf(txBuf, sizeof(txBuf), "%.4f %.4f %.4f\r\n",
          potValues[0], potValues[1], potValues[2]);

      // Transmit
      HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
    }
  }
}
