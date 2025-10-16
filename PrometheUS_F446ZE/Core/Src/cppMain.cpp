#include "cppMain.hpp"

extern "C" {
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
}

#include <atomic>

enum SystemState : uint8_t
{
  SYS_INIT = 0,
  SYS_IDLE,
  SYS_RUN,
  SYS_ERROR
};
volatile SystemState g_eSystemState = SystemState::SYS_INIT;

void printToConsole(const uint8_t* pData, uint16_t uSize);
void printDMAValue(uint16_t uADC_Value);
// void printDMABufferValues(uint16_t uADC_Buffer[3]);

// Flags

// System level state machine

std::atomic<bool> g_bFlagButtonPressed { false }; // Switch between IDLE <-> RUN
std::atomic<bool> g_bFlagError { false };         // Switch to ERROR


volatile bool g_bFlagPotentiometerSamplingCompleted = false;
volatile bool g_bFlagClutchTemperatureSamplingCompleted = false;
volatile bool g_bFlagClutchCurrentSamplingCompleted = false;

volatile bool g_bFlagUpdatePWMDutyCycles = false;



volatile uint8_t g_uADC_ClutchCurrentChannel = 0;



// ADC buffers
uint16_t g_uADC_ClutchCurrentBuffer[3] = {0};
uint16_t g_uADC_MotorVelocity = 0;

int cppMain()
{
  // ========== TIM2 ==========
  // Is used as the main timer to synchronize the events of the state machine
  // The main control loop runs at 1KHz, this timer triggers 10 times per 1KHz
  // The triggers are synchronized to be in the middle of the high pulse of the PWM signals

  // Pulse 1/10
  // ADC DMA is automatically triggered by OC2REF
  // Potentiometers       (ADC1 IN0, IN1, IN2)
  // Temperature sensors  (ADC2 IN7, IN8, IN9)
  // OC2REF triggers a function callback, in which ADC DMA for clutch current #1 is started (ADC3 IN4)
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);

  // Pulse 2/10
  // OC3REF triggers a function callback, in which ADC DMA for clutch current #2 is started (ADC3 IN5)
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3);

  // Pulse 3/10
  // OC4REF triggers a function callback, in which ADC DMA for clutch current #3 is started (ADC3 IN6)
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_4);

  // Pulse 10/10
  // OC1REF triggers a function callback, in which :
  // ADC DMA for the motor velocity is started (ADC3 IN7)
  // The PWM duty cycles for the clutches are updated
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);

  // ========== TIM3 ==========
  // Is an "intermediate" timer that TIM2 uses.
  // TIM3 triggers twice, at the middle of the high pulse of TIM1
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);

  // ========== TIM4 ==========
  // Motor PWM (Needs to be between 10Hz and 5KHz)
  // This timer is synchronized with TIM1, but runs at 1KHz instead of 10KHz
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

  // ========== TIM1 ==========
  // TIM1 is delayed to make sure all timers are synchronized together with it
  HAL_Delay(500);
  HAL_TIM_PWM_Start    (&htim1, TIM_CHANNEL_1); // Clutch #1 PWM A
  HAL_TIMEx_PWMN_Start (&htim1, TIM_CHANNEL_1); // Clutch #1 PWM B
  HAL_TIM_PWM_Start    (&htim1, TIM_CHANNEL_2); // Clutch #2 PWM A
  HAL_TIMEx_PWMN_Start (&htim1, TIM_CHANNEL_2); // Clutch #2 PWM B
  HAL_TIM_PWM_Start    (&htim1, TIM_CHANNEL_3); // Clutch #3 PWM A
  HAL_TIMEx_PWMN_Start (&htim1, TIM_CHANNEL_3); // Clutch #3 PWM B

  // ========== ADC ==========
  uint16_t uADC_PotentiometerBuffer[3]      = {0};
  uint16_t uADC_ClutchTemperatureBuffer[3]  = {0};

  // Triggered by TIM2's OC2REF, ADC1 DMA IN0, IN1, IN2
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)uADC_PotentiometerBuffer, 3);
  // Triggered by TIM2's OC2REF, ADC2 DMA IN7, IN8, IN9
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)uADC_ClutchTemperatureBuffer, 3);

  while (1)
  {
    switch (g_eSystemState)
    {
      case SystemState::SYS_INIT:
      {
        // Initialize all objects here ...
        g_eSystemState = SystemState::SYS_IDLE;
        break;
      }
      case SystemState::SYS_IDLE:
      {
        // If button is pressed, switch to RUN
        if (g_bFlagButtonPressed.exchange(false))
        {
          g_eSystemState = SystemState::SYS_RUN;
        }
        // Switch automatically in this case, no button is implemented yet
        g_eSystemState = SystemState::SYS_RUN;
        break;
      }
      case SystemState::SYS_RUN:
      {
        // If any error is detected, switch to ERROR
        if (g_bFlagError.exchange(false))
        {
          g_eSystemState = SystemState::SYS_ERROR;
        }
        // If button is pressed, switch to IDLE
        else if (g_bFlagButtonPressed.exchange(false))
        {
          g_eSystemState = SystemState::SYS_IDLE;
        }
        else
        {
          ControlLoop_1KHz();
        }
        break;
      }
      case SystemState::SYS_ERROR:
      {
        break;
      }
    }
  }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  // TIM2 interrupt callback
  if (htim->Instance == TIM2)
  {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

    // OC1REF
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      // Flag to update PWMs' duty cycles
      // g_bFlagUpdatePWMDutyCycles = true;

      // Read ADC3 DMA IN3 (motor velocity)
      // sConfig.Channel = ADC_CHANNEL_3; // ADC3_IN3
      // HAL_ADC_ConfigChannel(&hadc3, &sConfig);

      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&g_uADC_MotorVelocity, 1);
    }
    // OC2REF
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      // Read ADC3 DMA IN4 (clutch current #1)
      g_uADC_ClutchCurrentChannel = 0;
      sConfig.Channel = ADC_CHANNEL_4; // ADC3_IN4
      HAL_ADC_ConfigChannel(&hadc3, &sConfig);

      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&g_uADC_ClutchCurrentBuffer[0], 1);
    }
    // OC3REF
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      // Read ADC3 DMA IN5 (clutch current #2)
      g_uADC_ClutchCurrentChannel = 1;
      sConfig.Channel = ADC_CHANNEL_5; // ADC3_IN5
      HAL_ADC_ConfigChannel(&hadc3, &sConfig);

      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&g_uADC_ClutchCurrentBuffer[1], 1);
    }
    // OC4REF
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      // Read ADC3 DMA IN6 (clutch current #3)
      g_uADC_ClutchCurrentChannel = 2;
      sConfig.Channel = ADC_CHANNEL_6; // ADC3_IN6
      HAL_ADC_ConfigChannel(&hadc3, &sConfig);

      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&g_uADC_ClutchCurrentBuffer[2], 1);
    }
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    // Done reading potentiometer data
    // g_bFlagPotentiometerSamplingCompleted = true;
  }
  else if (hadc->Instance == ADC2)
  {
    // Done reading clutch temperature data
    // g_bFlagClutchTemperatureSamplingCompleted = true;
  }
  else if (hadc->Instance == ADC3 && g_uADC_ClutchCurrentChannel == 2)
  {
    // Done sampling all clutch current data
    // gbFlagClutchCurrentSamplingCompleted = true;
    g_uADC_ClutchCurrentChannel = 0;
  }
}

void printToConsole(const uint8_t* pData, uint16_t uSize)
{
  HAL_UART_Transmit(&huart2, pData, uSize, HAL_MAX_DELAY);
}

void printDMAValue(uint16_t uADC_Value)
{
  static char buffer[8];
  int len = sprintf(buffer, "%u\r\n", uADC_Value);
  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, HAL_MAX_DELAY);
}

/*
void printDMABufferValues(uint16_t uADC_Buffer[3])
{
  char tx_buffer[20];
  int tx_len = snprintf(
      tx_buffer,
      sizeof(tx_buffer),
      "%u %u %u\r\n",
      uADC_Buffer[0], uADC_Buffer[1], uADC_Buffer[2]
  );
  printToConsole((uint8_t*)tx_buffer, tx_len);
}
*/
