#include "TestsNamespace.hpp"
#include "main.h"

#include "DigitalInput.hpp"
#include <cstdio>
#include <cstring>

void Tests::button()
{
  DigitalInput button;
  button.init(SWRUN_GPIO_Port, SWRUN_Pin);

  // bool lastState = button.isOn();
  char txBuf[32];

  while (true)
  {
	/*
    bool currentState = button.isOn();

    if (currentState != lastState)
    {
      int len = snprintf(txBuf, sizeof(txBuf), "%s\r\n", currentState ? "ON" : "OFF");
      HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<const uint8_t*>(txBuf), len);
      lastState = currentState;
    }
    HAL_Delay(10);
    */

	bool state = button.isOn();
	int len = snprintf(txBuf, sizeof(txBuf), "%d\r\n", state);
    HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<const uint8_t*>(txBuf), len);
	HAL_Delay(100);
  }
}
