#include "cppMain.hpp"

extern "C" {
#include "main.h"
#include "stm32f4xx_hal.h"
}

uint16_t clutch_current_buffer[3] = {0};

int cppMain()
{
  // TIM2
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2); // ADC Trigger 1 (Potentiometers, temperature sensors, clutch current #1)
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3); // ADC Trigger 2 (Clutch current #2)
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_4); // ADC Trigger 3 (Clutch current #3)

  // TIM3 (Is an "intermediate" timer that TIM2 uses. TIM3 twice, at the middle of the high pulse of TIM1)
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);

  // TIM4
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); // Clutch #2 PWM #1
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2); // Clutch #2 PWM #2
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3); // Clutch #3 PWM #1
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4); // Clutch #4 PWM #2

  // TIM1
  HAL_Delay(500); // Delay to make sure all timers are synchronized together)
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // Motor PWM
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // Clutch #1 PWM #1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); // Clutch #1 PWM #2

  // ADCx
  uint16_t potentiometer_buffer[3]      = {0};
  uint16_t clutch_temperature_buffer[3] = {0};

  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)potentiometer_buffer, 3);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)clutch_temperature_buffer, 3);

  while (1)
  {

  }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  // TIM2 interrupt callbacks
  if (htim->Instance == TIM2)
  {
    // OC2REF
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&clutch_current_buffer[0], 1);
    }
    // OC3REF
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&clutch_current_buffer[1], 1);
    }
    // OC4REF
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
    {
      HAL_ADC_Start_DMA(&hadc3, (uint32_t*)&clutch_current_buffer[2], 1);
    }
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    // Done reading potentiometer data
  }
  else if (hadc->Instance == ADC2)
  {
    // Done reading clutch temperature data
  }
}
