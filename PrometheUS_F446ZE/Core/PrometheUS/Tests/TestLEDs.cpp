#include "TestsNamespace.hpp"
#include "main.h"

#include "LED.hpp"

void Tests::LEDs()
{
  LED ledRun;
  LED ledError;

  ledRun.init(DEL_1_GPIO_Port, DEL_1_Pin);
  ledError.init(DEL_2_GPIO_Port, DEL_2_Pin);

  const uint32_t DELAY_MS = 1000;

  while (true)
  {
    ledRun.toggle();
    ledError.toggle();

    HAL_Delay(DELAY_MS);
  }
}
