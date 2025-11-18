#include "TestsNamespace.hpp"
#include "main.h"

#include "Motor.hpp"
#include "StateMachines.hpp"
#include <cstdio>

void Tests::motor(SystemFlags* systemFlags, MotorVelocitySampler* motorVelocitySampler)
{
  Motor motor(motorVelocitySampler);
  motor.init();

  motor.start();

  motor.setRPM(403);

  while (true)
  {
	HAL_Delay(100);
  }

  /*
  SystemState systemState = SystemState::SYS_RUN;
  ControlState controlState = ControlState::CONTROL_WAIT_SAMPLING;

  const uint32_t RUNNING_PERIOD_MS = 1000;
  uint32_t counter = 0;
  bool running = true;

  char txBuf[32];

  while (true)
  {
    if (systemState == SystemState::SYS_RUN)
    {
      if (controlState == ControlState::CONTROL_WAIT_SAMPLING)
      {
        if (systemFlags->motorVelocityDone.exchange(false))
        {
          controlState = ControlState::CONTROL_CALCULATE_CONTROL_LAWS;
        }
      }
      else if (controlState == ControlState::CONTROL_CALCULATE_CONTROL_LAWS)
      {
        if (counter % RUNNING_PERIOD_MS == 0)
        {
          if (running)
          {
            running = false;
            motor.stop();
          }
          else
          {
            running = true;
            motor.start();
          }
        }

        float motorVelocity = motor.getMotorVelocity();

        int len = snprintf(txBuf, sizeof(txBuf), "%0.4f\r\n", motorVelocity);
        HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
      }
      counter++;
    }
    else if (systemState == SystemState::SYS_ERROR)
    {

    }
  }
  */
}
