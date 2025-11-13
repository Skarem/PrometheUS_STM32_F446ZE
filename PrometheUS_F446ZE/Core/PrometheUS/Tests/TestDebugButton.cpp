#include "TestsNamespace.hpp"
#include "main.h"

#include "DebugButton.hpp"

#include <cstdio>

void Tests::buttonDebug()
{
  DebugButton button;
  button.init(DEBUG_BUTTON_GPIO_Port, DEBUG_BUTTON_Pin);

  char txBuf[8];
  uint16_t counter = 0;

  while (true)
  {
    if (button.pressed())
    {
      int len = snprintf(txBuf, sizeof(txBuf), "%d\r\n", counter++);
      HAL_UART_Transmit_DMA(&huart3, reinterpret_cast<uint8_t*>(txBuf), len);
      HAL_Delay(1);
    }
  }
}
