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
void Tests::clutchCurrents(SystemFlags* systemFlags,
    ClutchCurrentSampler* clutchCurrentSampler1,
    ClutchCurrentSampler* clutchCurrentSampler2,
    ClutchCurrentSampler* clutchCurrentSampler3)
{

  HAL_GPIO_WritePin(Clutch_1_PWM_B_GPIO_Port, Clutch_1_PWM_B_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Clutch_2_PWM_B_GPIO_Port, Clutch_2_PWM_B_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Clutch_3_PWM_B_GPIO_Port, Clutch_3_PWM_B_Pin, GPIO_PIN_RESET);

  PWMComplementary pwmClutch1;
  PWMComplementary pwmClutch2;
  PWMComplementary pwmClutch3;

  pwmClutch1.init(&htim1, TIM_CHANNEL_1);
  pwmClutch2.init(&htim1, TIM_CHANNEL_2);
  pwmClutch3.init(&htim1, TIM_CHANNEL_3);

  const float DUTY_CYCLE_TEST = 0.57f;

  pwmClutch1.update(DUTY_CYCLE_TEST);
  pwmClutch2.update(DUTY_CYCLE_TEST);
  pwmClutch3.update(DUTY_CYCLE_TEST);

  PrometheUS_Gripper::startTimers();

  pwmClutch1.start();
  pwmClutch2.start();
  pwmClutch3.start();

  clutchCurrentSampler1->init(&hadc3, ADC_CHANNEL_4, FINGER_1_INDEX);
  clutchCurrentSampler2->init(&hadc3, ADC_CHANNEL_5, FINGER_2_INDEX);
  clutchCurrentSampler3->init(&hadc3, ADC_CHANNEL_6, FINGER_3_INDEX);

  uint32_t counter = 0;
  const uint32_t PERIOD_WAIT_ITER = 100;

  char txBuf[64];
  float currValues[3];

  while (true)
  {
    // Wait for all currents read completion flag
    uint8_t flag = systemFlags->adcDoneMask.exchange(0, std::memory_order_relaxed);
    if (flag & SystemFlags::CURR)
    {
      // Convert raw ADC values to floats
      currValues[0] = clutchCurrentSampler1->convertRawAdcValue();
      currValues[1] = clutchCurrentSampler2->convertRawAdcValue();
      currValues[2] = clutchCurrentSampler3->convertRawAdcValue();

      if (++counter % PERIOD_WAIT_ITER == 0)
      {
        // Format into human-readable ASCII
        // int len = snprintf(txBuf, sizeof(txBuf), "%.2f %.2f %.2f\r\n", currValues[0], currValues[1], currValues[2]);
        int len = snprintf(txBuf, sizeof(txBuf), "%.2f\r\n", currValues[0]);

        // Transmit
        HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
      }
    }
  }
}
