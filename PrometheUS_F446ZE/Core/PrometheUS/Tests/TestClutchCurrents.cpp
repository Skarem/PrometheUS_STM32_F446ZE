#include "TestsNamespace.hpp"
#include "main.h"

#include "PrometheUS.hpp"
#include <cstdio>

/**
 * @file  TestClutchCurrent.cpp
 * @brief Unit test for ADC3 clutch currents channel swapping sampling
 *
 * @details
 * This test validates the correct operation of the ClutchCurrentSampler class,
 * which performs ADC3 channel swapping and sampling of one clutch current,
 * triggered by TIM2's CH1, CH2, or CH3. The current is read in the middle of
 * PWM high pulse
 *
 * The converted values are continuously transmitted over UART3 as human-readable floats
 *
 * Must disable : RUN_INTERRUPT_CALLBACKS
 * Must enable  : TEST_CURRENTS
 *
 * ADC3_IN4 -> PF6  (ADC_Current1)
 * ADC3_IN5 -> PF7  (ADC_Current2)
 * ADC3_IN6 -> PF8  (ADC_Current3)
 */
void Tests::clutchCurrents(SystemFlags &systemFlags,
    ClutchCurrentSampler &clutchCurrentSampler1,
    ClutchCurrentSampler &clutchCurrentSampler2,
    ClutchCurrentSampler &clutchCurrentSampler3)
{
  clutchCurrentSampler1.init(&hadc3, ADC_CHANNEL_4, FINGER_1_INDEX);
  clutchCurrentSampler2.init(&hadc3, ADC_CHANNEL_5, FINGER_2_INDEX);
  clutchCurrentSampler3.init(&hadc3, ADC_CHANNEL_6, FINGER_3_INDEX);

  PrometheUS_Gripper::startTimers();
  // TIM2 and TIM3 need TIM1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  char txBuf[64];
  float currValues[3];

  while (true)
  {
    // Wait for all currents read completion flag
    uint8_t flag = systemFlags.adcDoneMask.exchange(0, std::memory_order_relaxed);
    if (flag & SystemFlags::CURR)
    {
      // Convert raw ADC values to floats
      currValues[0] = clutchCurrentSampler1.convertRawAdcValue();
      currValues[1] = clutchCurrentSampler2.convertRawAdcValue();
      currValues[2] = clutchCurrentSampler3.convertRawAdcValue();

      // Format into human-readable ASCII
      int len = snprintf(txBuf, sizeof(txBuf), "%.2f %.2f %.2f\r\n",
          currValues[0], currValues[1], currValues[2]);

      // Transmit
      HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
    }
  }
}
