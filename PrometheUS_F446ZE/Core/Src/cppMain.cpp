#include "cppMain.hpp"

extern "C" {
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
}

#include <atomic>

#include "EncoderSampler.hpp"

/*
enum SystemState : uint8_t
{
  SYS_INIT = 0,
  SYS_IDLE,
  SYS_RUN,
  SYS_ERROR
};

enum ControlState : uint8_t
{
  CONTROL_START_SAMPLING = 0,
  CONTROL_WAIT_SAMPLING,
  CONTROL_CALCULATE_CONTROL_LAWS,
  CONTROL_FINISHED_CONTROL_LAWS,
  CONTROL_UPDATE_PWM_DUTY_CYCLES,
  CONTROL_FINISHED_CYCLE
};
*/

void ControlStateMachine();
void printValue(uint16_t uValue);

// ========== Flags ==========

// ===== System level state machine =====

/*
std::atomic<bool> g_bFlagButtonPressed { false }; // Switch between IDLE <-> RUN
std::atomic<bool> g_bFlagError { false };         // Switch to ERROR

// Low level state machine =====

std::atomic<bool> g_bFlagStartControlCycle { false };

std::atomic<bool> g_bFlagPotentiometerSamplingCompleted { false };
std::atomic<bool> g_bFlagClutchTemperatureSamplingCompleted { false };
std::atomic<bool> g_bFlagClutchCurrentSamplingCompleted { false };

std::atomic<bool> g_bFlagUpdatePWMDutyCycles { false };

static volatile uint8_t g_uADC_ClutchCurrentChannelIndex = 0;

// ========== ADC buffers ==========
uint16_t g_uADC_MotorVelocity = 0;
uint16_t g_uADC_ClutchCurrentBuffer[3] = {0};

// ========== ADC Configurations ==========

const uint32_t ADC_ConfigurationRank = 1;
const uint32_t ADC_ConfigurationSamplingTime = ADC_SAMPLETIME_3CYCLES;

static ADC_ChannelConfTypeDef g_sConfigMotorVelocity =
{
    .Channel = ADC_CHANNEL_3, // Read ADC3 DMA IN3 (motor velocity)
    .Rank = ADC_ConfigurationRank,
    .SamplingTime = ADC_ConfigurationSamplingTime,
    .Offset = 0
};

static ADC_ChannelConfTypeDef g_sConfigClutchCurrent1 =
{
    .Channel = ADC_CHANNEL_4,  // Read ADC3 DMA IN4 (clutch current #1)
    .Rank = ADC_ConfigurationRank,
    .SamplingTime = ADC_ConfigurationSamplingTime,
    .Offset = 0
};

static ADC_ChannelConfTypeDef g_sConfigClutchCurrent2 =
{
    .Channel = ADC_CHANNEL_5, // Read ADC3 DMA IN5 (clutch current #2)
    .Rank = ADC_ConfigurationRank,
    .SamplingTime = ADC_ConfigurationSamplingTime,
    .Offset = 0
};

static ADC_ChannelConfTypeDef g_sConfigClutchCurrent3 =
{
    .Channel = ADC_CHANNEL_6, // Read ADC3 DMA IN6 (clutch current #3)
    .Rank = ADC_ConfigurationRank,
    .SamplingTime = ADC_ConfigurationSamplingTime,
    .Offset = 0
};

// ========== PWM Test ==========

#define ARR_VALUE 1000
#define STEP_PERCENT 10
#define PAUSE_CYCLES 10

struct PWM
{
  TIM_HandleTypeDef* m_htim;
  uint32_t m_channel;
  uint16_t m_dutyCycle; // Current duty cycle (0..ARR)
  bool m_error;
  // For debug only
  uint16_t m_step;      // Step function
  uint8_t pauseCounter;

  PWM(TIM_HandleTypeDef* _htim, uint32_t _channel, uint16_t _dutyCycle)
  {
    m_htim = _htim;
    m_channel = _channel;
    m_dutyCycle = _dutyCycle;
    m_error = false;
    m_step = ARR_VALUE * STEP_PERCENT / 100;
    pauseCounter = 0;
  }
};

struct ClutchPWMs
{
  PWM Clutch1;
  PWM Clutch2;
  PWM Clutch3;

  ClutchPWMs() :
    Clutch1({ &htim1, TIM_CHANNEL_1, 1 * ARR_VALUE / 4 }),
    Clutch2({ &htim1, TIM_CHANNEL_2, 2 * ARR_VALUE / 4 }),
    Clutch3({ &htim1, TIM_CHANNEL_3, 3 * ARR_VALUE / 4 }) {}

  void Error()
  {
    Clutch1.m_error = true;
    Clutch2.m_error = true;
    Clutch3.m_error = true;
  }

  void Reset()
  {
    Clutch1.m_error = false;
    Clutch2.m_error = false;
    Clutch3.m_error = false;
  }
};

ClutchPWMs g_clutchPWMs;

// ========== Encoders ==========

// EncoderSampler g_encoderSampler1(&hspi1, Encoder_CS_1_GPIO_Port, Encoder_CS_1_Pin);
// EncoderSampler g_encoderSampler2(&hspi1, Encoder_CS_2_GPIO_Port, Encoder_CS_2_Pin);
// EncoderSampler g_encoderSampler3(&hspi1, Encoder_CS_3_GPIO_Port, Encoder_CS_3_Pin);

// ==============================

*/

int cppMain()
{
  EncoderSampler encoder1(&hspi1, Encoder_CS_1_GPIO_Port, Encoder_CS_1_Pin);

  while (1)
  {
    encoder1.sampleRawValue();
    uint16_t pos = encoder1.convertValue();
    printValue(pos);
    HAL_Delay(250);
  }

  /*
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

  SystemState eSystemState = SystemState::SYS_INIT;

  while (1)
  {
    switch (eSystemState)
    {
      case SystemState::SYS_INIT:
      {
        // Initialize all objects here ...
        g_encoderSampler1.initOffsetPosition();
        // g_encoderSampler2.initOffsetPosition();
        // g_encoderSampler3.initOffsetPosition();

        eSystemState = SystemState::SYS_IDLE;
        break;
      }
      case SystemState::SYS_IDLE:
      {
        // // If button is pressed, switch to RUN
        // if (g_bFlagButtonPressed.exchange(false))
        // {
        //   eSystemState = SystemState::SYS_RUN;
        // }

        // Switch automatically in this case, no button is implemented yet
        eSystemState = SystemState::SYS_RUN;
        break;
      }
      case SystemState::SYS_RUN:
      {
        // If any error is detected, switch to ERROR
        if (g_bFlagError.exchange(false))
        {
          eSystemState = SystemState::SYS_ERROR;
        }
        // If button is pressed, switch to IDLE
        else if (g_bFlagButtonPressed.exchange(false))
        {
          eSystemState = SystemState::SYS_IDLE;
        }
        else
        {
          ControlStateMachine();
        }
        break;
      }
      case SystemState::SYS_ERROR:
      {
        break;
      }
    }
  }
  */
}

/*

void ControlStateMachine()
{
  static ControlState eControlState = ControlState::CONTROL_FINISHED_CYCLE;

  switch (eControlState)
  {
    case ControlState::CONTROL_START_SAMPLING:
    {
      // All ADC sampling is triggered by timers

      // Sample encoders
      g_encoderSampler1.sampleRawValue();
      // g_encoderSampler2.sampleRawValue();
      // g_encoderSampler3.sampleRawValue();

      uint16_t encoderPosition1 = g_encoderSampler1.convertValue();
      printValue(encoderPosition1);

      eControlState = ControlState::CONTROL_WAIT_SAMPLING;
      break;
    }
    case ControlState::CONTROL_WAIT_SAMPLING:
    {
      // Waiting for all ADC transmissions to be done
      if (g_bFlagPotentiometerSamplingCompleted.exchange(false) &&
          g_bFlagClutchTemperatureSamplingCompleted.exchange(false) &&
          g_bFlagClutchCurrentSamplingCompleted.exchange(false))
      {
        eControlState = ControlState::CONTROL_CALCULATE_CONTROL_LAWS;
      }
      break;
    }
    case ControlState::CONTROL_CALCULATE_CONTROL_LAWS:
    {
      // Convert raw ADC and SPI data
      // ...

      // High-level control laws
      // ...

      // Low-level clutch current PIDs
      // PWM test

      eControlState = ControlState::CONTROL_FINISHED_CONTROL_LAWS;
      break;
    }
    case ControlState::CONTROL_FINISHED_CONTROL_LAWS:
    {
      // Waiting for the timer trigger to update PWM duty cycles
      if (g_bFlagUpdatePWMDutyCycles.exchange(false))
      {
        eControlState = ControlState::CONTROL_UPDATE_PWM_DUTY_CYCLES;
      }
      break;
    }
    case ControlState::CONTROL_UPDATE_PWM_DUTY_CYCLES:
    {
      // Update PWM duty cycles with new values calculated in CONTROLE_CALCULATE_LAWS via burst DMA
      // ...

      eControlState = ControlState::CONTROL_FINISHED_CYCLE;
      break;
    }
    case ControlState::CONTROL_FINISHED_CYCLE:
    {
      // Done updating, cycle completed
      // Waiting for timer trigger to start next cycle
      if (g_bFlagStartControlCycle.exchange(false))
      {
        eControlState = ControlState::CONTROL_START_SAMPLING;
      }
      break;
    }
  }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  // TIM2 interrupt callback
  if (htim->Instance == TIM2)
  {
    // OC1REF
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
      // Flag to update PWMs' duty cycles
      g_bFlagUpdatePWMDutyCycles = true;

      // Read ADC3 DMA IN3 (motor velocity)
      HAL_ADC_ConfigChannel(&hadc3, &g_sConfigMotorVelocity);
      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&g_uADC_MotorVelocity, 1);
    }
    // OC2REF
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      // Start control cycle
      g_bFlagStartControlCycle = true;

      // Read ADC3 DMA IN4 (clutch current #1)
      g_uADC_ClutchCurrentChannelIndex = 0;
      HAL_ADC_ConfigChannel(&hadc3, &g_sConfigClutchCurrent1);
      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&g_uADC_ClutchCurrentBuffer[g_uADC_ClutchCurrentChannelIndex], 1);
    }
    // OC3REF
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      // Read ADC3 DMA IN5 (clutch current #2)
      g_uADC_ClutchCurrentChannelIndex = 1;
      HAL_ADC_ConfigChannel(&hadc3, &g_sConfigClutchCurrent2);
      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&g_uADC_ClutchCurrentBuffer[g_uADC_ClutchCurrentChannelIndex], 1);
    }
    // OC4REF
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      // Read ADC3 DMA IN6 (clutch current #3)
      g_uADC_ClutchCurrentChannelIndex = 2;
      HAL_ADC_ConfigChannel(&hadc3, &g_sConfigClutchCurrent3);
      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&g_uADC_ClutchCurrentBuffer[g_uADC_ClutchCurrentChannelIndex], 1);
    }
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    // Done reading potentiometer data
    g_bFlagPotentiometerSamplingCompleted = true;
  }
  else if (hadc->Instance == ADC2)
  {
    // Done reading clutch temperature data
    g_bFlagClutchTemperatureSamplingCompleted = true;
  }
  else if (hadc->Instance == ADC3 && g_uADC_ClutchCurrentChannelIndex == 2)
  {
    // Done sampling all clutch current data
    g_uADC_ClutchCurrentChannelIndex = 0;
    g_bFlagClutchCurrentSamplingCompleted = true;
  }
}
*/

void printValue(uint16_t uValue)
{
  static char buffer[8];
  int len = sprintf(buffer, "%u\r\n", uValue);
  HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, HAL_MAX_DELAY);
}

/*
 void printValue(float value)
 {
   static char buffer[16];
   int len = sprintf(buffer, "%.2f\r\n", value);
   HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, HAL_MAX_DELAY);
 }
 */
