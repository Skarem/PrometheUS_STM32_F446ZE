#include "TestsNamespace.hpp"

#include "TelemetrySender.hpp"

/**
 * @file  TestSenderError.cpp
 * @brief Unit test for TelemetrySender when in error mode
 *
 * @details
 * Loops through all error messages possible
 *
 * Transmit data over UART3 with the USB cable
 *
 * USART3_TX -> PD8
 * USART3_RX -> PD9 (not needed)
 */
void Tests::senderErrors()
{
  TelemetrySender sender;
  sender.init(&huart3);

  const float BASE_POTS[FINGER_COUNT] = { 60.0f, 120.0f, 180.0f };
  const float BASE_TEMP[FINGER_COUNT] = { 30.0f, 40.0f, 50.0f };
  const float BASE_CURR[FINGER_COUNT] = { 0.5f, 1.0f, 1.5f };
  const float BASE_MOTOR              = 3500.0f;

  const bool IN_ERROR = true;

  const uint32_t SEND_PERIOD_MS = 1000; // 1 Hz

  uint8_t errorSource = static_cast<uint8_t>(ErrorSource::CLUTCH_TEMPERATURE_1);
  const uint8_t lastError = static_cast<uint8_t>(ErrorSource::LAST_INDEX) - 1;

  while (true)
  {
    uint32_t now = HAL_GetTick();
    sender.send(now, BASE_POTS, BASE_TEMP, BASE_CURR, BASE_MOTOR, IN_ERROR, errorSource);

    if (++errorSource > lastError)
    {
      errorSource = static_cast<uint8_t>(ErrorSource::CLUTCH_TEMPERATURE_1);
    }

    HAL_Delay(SEND_PERIOD_MS);
  }
}
