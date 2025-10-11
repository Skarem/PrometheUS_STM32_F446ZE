#include "cppMain.hpp"

extern "C" {
#include "main.h"
#include "stm32f4xx_hal.h"
}

int cppMain()
{

  uint16_t adc1_values[3] = {0};

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_2);

  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3);

  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_values, 3);

  while (1)
  {

  }
}

/*
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
  }
}
*/
