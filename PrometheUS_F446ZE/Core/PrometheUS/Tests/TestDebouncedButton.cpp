#include "DebouncedButton.hpp"
#include "TestsNamespace.hpp"
#include "main.h"

#include "DigitalOutput.hpp"
#include "PrometheUS.hpp"

#include <cstdio>

void Tests::buttonDebug(SystemFlags* systemFlags)
{
  DebouncedButton button;
  button.init(Button_Software_Run_GPIO_Port, Button_Software_Run_Pin);

  // DigitalOutput debugPin;
  // debugPin.init(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);

  PrometheUS_Gripper::startTimers();
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  char txBuf[8];
  uint16_t counter = 0;

  while (true)
  {
    if (systemFlags->startControlCycle.exchange(false))
    {
      // debugPin.pulse();
      if (button.pressed())
      {
        int len = snprintf(txBuf, sizeof(txBuf), "%d\r\n", counter++);
        HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
      }
    }
  }
}
