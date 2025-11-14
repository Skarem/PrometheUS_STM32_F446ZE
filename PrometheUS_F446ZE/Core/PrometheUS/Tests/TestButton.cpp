#include "TestsNamespace.hpp"
#include "main.h"

#include "DigitalInput.hpp"
#include "DigitalOutput.hpp"
#include "PrometheUS.hpp"

#include <cstdio>
#include <cstring>

static bool debouncing = false;
static uint8_t counter = 0;
static const uint8_t DEBOUNCING_ITER = 5;

static uint16_t counterButtonPress = 0;

void update(SystemFlags* systemFlags)
{
  // If callback detected a real press
  if (systemFlags->buttonPressed.exchange(false))
  {
    debouncing = true;
    counter = 0;

    char txBuf[8];
    int len = snprintf(txBuf, sizeof(txBuf), "%d\r\n", ++counterButtonPress);
    HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t*>(txBuf), len, HAL_MAX_DELAY);
  }

  if (debouncing)
  {
    if (++counter >= DEBOUNCING_ITER)
    {
      // Re-enable EXTI back
      EXTI->IMR |= (1u << __builtin_ctz(Button_Software_Run_Pin));
      debouncing = false;
    }
  }
}

void Tests::button(SystemFlags* systemFlags)
{
  // DigitalOutput debugPin;
  // debugPin.init(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);

  PrometheUS_Gripper::startTimers();
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  while (true)
  {
    update(systemFlags);
    if (systemFlags->startControlCycle.exchange(false))
    {
      // debugPin.pulse();
    }
  }
}
