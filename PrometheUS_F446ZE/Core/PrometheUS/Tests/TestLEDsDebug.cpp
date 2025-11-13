#include "TestsNamespace.hpp"
#include "main.h"

#include "DigitalOutput.hpp"

void Tests::LEDsDebug()
{
  DigitalOutput led1;
  DigitalOutput led2;

  led1.init(LED_Debug_Run_GPIO_Port, LED_Debug_Run_Pin);
  led2.init(LED_Debug_Error_GPIO_Port, LED_Debug_Error_Pin);

  led1.off();
  led2.off();

  const uint32_t DELAY = 500;

  while (true)
  {
    led1.on();

    HAL_Delay(DELAY);

    led1.off();
    led2.on();

    HAL_Delay(DELAY);

    led2.off();

    HAL_Delay(DELAY);
  }
}
