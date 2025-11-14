#include "TestsNamespace.hpp"
#include "main.h"

#include "ClutchesTemperatureSampler.hpp"
#include "PrometheUS.hpp"
#include <cstdio>

/**
 * @file  TestClutchTemperature.cpp
 * @brief Unit test for ADC2 clutch temperatures burst sampling
 *
 * @details
 * This test validates the correct operation of the ClutchesTemperatureSampler class,
 * which performs ADC2 burst conversions of three potentiometer channels triggered by TIM2 CH1
 *
 * The converted values are continuously transmitted over UART3 as human-readable floats
 *
 * Must disable : RUN_INTERRUPT_CALLBACKS
 * Must enable  : TEST_TEMPERATURES
 *
 * ADC2_IN7 -> PA7  (ADC_Temp1)
 * ADC2_IN8 -> PB0  (ADC_Temp2)
 * ADC2_IN9 -> PB1  (ADC_Temp3)
 */
void Tests::clutchTemperatures(SystemFlags* systemFlags)
{
  ClutchesTemperatureSampler clutchTemperatures;
  clutchTemperatures.init(&hadc2);

  PrometheUS_Gripper::startTimers();
  // TIM2 and TIM3 need TIM1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  char txBuf[64];
  float tempValues[3];

  uint32_t counter = 0;
  const uint32_t PERIOD_WAIT_ITER = 100;

  while (true)
  {
    // Wait for ADC2 conversion completion flag
    uint8_t flag = systemFlags->adcDoneMask.exchange(0, std::memory_order_relaxed);
    if (flag & SystemFlags::TEMP)
    {
      // Convert raw ADC values to floats
      clutchTemperatures.convertAll(tempValues);

      if ((++counter % PERIOD_WAIT_ITER) == 0)
      {
        // Format into human-readable ASCII
        int len = snprintf(txBuf, sizeof(txBuf), "%.2f %.2f %.2f\r\n", tempValues[0], tempValues[1], tempValues[2]);

        // Transmit
        HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
      }
    }
  }
}
