#include "TestsNamespace.hpp"
#include "main.h"

#include "ClutchesTemperatureSampler.hpp"

#include "DefineConstants.hpp"
#include <cstdio>

void Tests::adcToTempConversion()
{
  const uint16_t ADC_MAX_COUNT = static_cast<uint16_t>(AdcUtils::ADC_MAXIMAL_COUNT_VALUE);
  char txBuf[64];

  while (true)
  {
    for (uint16_t adcCount = 0; adcCount <= ADC_MAX_COUNT; ++adcCount)
    {
      float voltage_volt = AdcUtils::getAdcVoltage(adcCount);
      float temperature_celsius = ClutchesTemperatureSampler::testConversion(voltage_volt);

      int len = snprintf(txBuf, sizeof(txBuf), "%d; %.4f; %.4f\r\n", adcCount, voltage_volt, temperature_celsius);
      HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t*>(txBuf), len, HAL_MAX_DELAY);

      HAL_Delay(10);
    }
    HAL_Delay(1000);
  }
}
