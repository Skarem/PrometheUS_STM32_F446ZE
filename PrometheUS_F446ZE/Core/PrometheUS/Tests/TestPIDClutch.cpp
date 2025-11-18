#include "TestsNamespace.hpp"
#include "main.h"

enum PIDStateMachine : uint8_t
{
    WAIT_CURRENT_SAMPLING = 0,
    CALCULATE,
    FINISHED_CALCULATE,
    UPDATE_PWM
};

#include "Clutch.hpp"
#include "PrometheUS.hpp"
#include <cstdio>

void Tests::PIDClutch(SystemFlags* systemFlags,
    ClutchCurrentSampler* currentSampler1,
    ClutchCurrentSampler* currentSampler2,
    ClutchCurrentSampler* currentSampler3)
{
  Clutch clutch1(currentSampler1);
  Clutch clutch2(currentSampler2);
  Clutch clutch3(currentSampler3);

  clutch1.init(ADC_CHANNEL_4, FINGER_1_INDEX, TIM_CHANNEL_1, Clutch_1_PWM_B_GPIO_Port, Clutch_1_PWM_B_Pin);
  clutch1.init(ADC_CHANNEL_5, FINGER_2_INDEX, TIM_CHANNEL_2, Clutch_2_PWM_B_GPIO_Port, Clutch_2_PWM_B_Pin);
  clutch1.init(ADC_CHANNEL_6, FINGER_3_INDEX, TIM_CHANNEL_3, Clutch_3_PWM_B_GPIO_Port, Clutch_3_PWM_B_Pin);

  PrometheUS_Gripper::startTimers();

  clutch1.start();
  clutch2.start();
  clutch3.start();

  PIDStateMachine state = PIDStateMachine::WAIT_CURRENT_SAMPLING;

  float measuredValues[FINGER_COUNT];
  float targetValues[FINGER_COUNT] = { 0.25f, 0.5f, 0.75f };
  float calculatedPIDValues[FINGER_COUNT];

  char txBuf[128];

  while (true)
  {
    switch (state)
    {
      case WAIT_CURRENT_SAMPLING:
      {
        // Wait for all currents read completion flag
        uint8_t flag = systemFlags->adcDoneMask.exchange(0, std::memory_order_relaxed);
        if (flag & SystemFlags::CURR)
        {
          state = PIDStateMachine::CALCULATE;
        }
        break;
      }
      case CALCULATE:
      {
        // Convert raw ADC values to floats
        measuredValues[FINGER_1_INDEX] = clutch1.getMeasuredCurrent();
        measuredValues[FINGER_2_INDEX] = clutch2.getMeasuredCurrent();
        measuredValues[FINGER_3_INDEX] = clutch3.getMeasuredCurrent();

        // Calculate PIDs
        calculatedPIDValues[FINGER_1_INDEX] = clutch1.calculateCurrentPID(targetValues[FINGER_1_INDEX], measuredValues[FINGER_1_INDEX]);
        calculatedPIDValues[FINGER_2_INDEX] = clutch2.calculateCurrentPID(targetValues[FINGER_2_INDEX], measuredValues[FINGER_2_INDEX]);
        calculatedPIDValues[FINGER_3_INDEX] = clutch3.calculateCurrentPID(targetValues[FINGER_3_INDEX], measuredValues[FINGER_3_INDEX]);

        int len = snprintf(txBuf, sizeof(txBuf), "%.2f %.2f %.2f %.3f %.3f %.3f\r\n",
            measuredValues[FINGER_1_INDEX],
            measuredValues[FINGER_2_INDEX],
            measuredValues[FINGER_3_INDEX],
            calculatedPIDValues[FINGER_1_INDEX],
            calculatedPIDValues[FINGER_2_INDEX],
            calculatedPIDValues[FINGER_3_INDEX]);

        HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);

        state = PIDStateMachine::FINISHED_CALCULATE;
        break;
      }
      case FINISHED_CALCULATE:
      {
        // Waiting for the timer trigger to update PWM duty cycles
        if (systemFlags->updatePwmDutyCycles.exchange(false))
        {
          state = PIDStateMachine::UPDATE_PWM;
        }
        break;
      }
      case UPDATE_PWM:
      {
        clutch1.updateCommand();
        clutch1.updateCommand();
        clutch1.updateCommand();

        state = PIDStateMachine::WAIT_CURRENT_SAMPLING;
        break;
      }
    }
  }
}
