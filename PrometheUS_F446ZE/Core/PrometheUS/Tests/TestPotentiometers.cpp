#include <TestsNamespace.hpp>
#include "main.h"

#include "PotentiometersSampler.hpp"
#include "PrometheUS.hpp"
#include <cstdio>

/**
 * @file  TestPotentiometers.cpp
 * @brief Unit test for ADC1 potentiometer burst sampling
 *
 * @details
 * This test validates the correct operation of the PotentiometersSampler class,
 * which performs ADC1 burst conversions of three potentiometer channels triggered by TIM2 CH1
 *
 * The converted values are continuously transmitted over UART3 as human-readable floats
 *
 * Must disable : RUN_INTERRUPT_CALLBACKS
 * Must enable  : TEST_POTENTIOMETERS
 *
 * ADC1_IN0 -> PA0  (ADC_Pot1)
 * ADC1_IN1 -> PA1  (ADC_Pot2)
 * ADC1_IN2 -> PA2  (ADC_Pot3)
 */
void Tests::potentiometers(SystemFlags* systemFlags)
{
  PotentiometersSampler potentiometers;
  potentiometers.init(&hadc1);

  PrometheUS_Gripper::startTimers();
  // TIM2 and TIM3 need TIM1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  char txBuf[64];
  float potValues[3];

  uint32_t counter = 0;
  const uint32_t PERIOD_WAIT_ITER = 100;

  while (true)
  {
    // Wait for ADC1 conversion completion flag
    uint8_t flag = systemFlags->adcDoneMask.exchange(0, std::memory_order_relaxed);
    if (flag & SystemFlags::POTS)
    {
      // Convert raw ADC values to floats
      potentiometers.convertAll(potValues);

      if (++counter % PERIOD_WAIT_ITER == 0)
      {
        // Format into human-readable ASCII
        // int len = snprintf(txBuf, sizeof(txBuf), "%.1f %.1f %.1f\r\n", potValues[0], potValues[1], potValues[2]);
        int len = snprintf(txBuf, sizeof(txBuf), "%.2f\r\n", potValues[0]);
        // Transmit
        HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
      }
    }
  }
}
