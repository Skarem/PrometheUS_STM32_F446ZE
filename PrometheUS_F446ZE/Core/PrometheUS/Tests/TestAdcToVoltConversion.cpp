#include "TestsNamespace.hpp"
#include "main.h"

#include "DefineConstants.hpp"
#include <cstdio>

void Tests::adcToVoltConversion()
{
  uint16_t adcMaxCount = static_cast<uint16_t>(AdcUtils::ADC_MAXIMAL_COUNT_VALUE);
  char txBuf[64];

  while (true)
  {
    for (uint16_t adcCount = 0; adcCount <= adcMaxCount; ++adcCount)
    {
      float voltage = AdcUtils::getAdcVoltage(adcCount);

      int len = snprintf(txBuf, sizeof(txBuf), "%d; %.4f\r\n", adcCount, voltage);
      HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t*>(txBuf), len, HAL_MAX_DELAY);

      HAL_Delay(10);
    }
    HAL_Delay(1000);
  }
}
