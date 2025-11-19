#include "TestsNamespace.hpp"
#include "main.h"

#include "Motor.hpp"
#include "StateMachines.hpp"
#include <cstdio>

#include "PrometheUS.hpp"

void Tests::motor(SystemFlags* systemFlags, MotorVelocitySampler* motorVelocitySampler)
{
  Motor motor(motorVelocitySampler);
  motor.init();

  PrometheUS_Gripper::startTimers();
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  motor.setRPM(250);
  motor.start();

  /*
  while (true)
  {
	HAL_Delay(10);
  }
  */

  ControlState controlState = ControlState::CONTROL_WAIT_SAMPLING;

  const uint32_t RUNNING_PERIOD_MS = 10000;
  uint32_t counter = 0;

  char txBuf[32];

  while (true)
  {
    if (controlState == ControlState::CONTROL_WAIT_SAMPLING)
    {
      if (systemFlags->motorVelocityDone.exchange(false))
      {
    	systemFlags->motorVelocityDone.store(false, std::memory_order_relaxed);
        controlState = ControlState::CONTROL_CALCULATE_CONTROL_LAWS;
      }
    }

    else if (controlState == ControlState::CONTROL_CALCULATE_CONTROL_LAWS)
    {
      if (++counter % RUNNING_PERIOD_MS == 0)
      {
        float motorVelocity = motor.getMotorVelocity();

        int len = snprintf(txBuf, sizeof(txBuf), "%0.1f\r\n", motorVelocity);
        HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
      }
    }
  }
}
