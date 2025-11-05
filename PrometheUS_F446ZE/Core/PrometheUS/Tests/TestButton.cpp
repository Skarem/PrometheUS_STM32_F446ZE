#include "TestsNamespace.hpp"
#include "main.h"

#include "DigitalInput.hpp"
#include <cstdio>
#include <cstring>

static void printUart3(bool state)
{
  static char txBuf[32];
  snprintf(txBuf, sizeof(txBuf), "%s\r\n", state ? "ON" : "OFF");
  HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<const uint8_t*>(txBuf), strlen(txBuf));
}

void Tests::button()
{
  DigitalInput button;
  button.init(SWRUN_GPIO_Port, SWRUN_Pin);

  bool lastState = button.isOn();
  printUart3(lastState);

  while (true)
  {
    bool currentState = button.isOn();

    if (currentState != lastState)
    {
      printUart3(currentState);
      lastState = currentState;
    }
    HAL_Delay(10);
  }
}
