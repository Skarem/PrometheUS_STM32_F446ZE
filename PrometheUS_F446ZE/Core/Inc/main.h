/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;

extern DAC_HandleTypeDef hdac;

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart2;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_Trigger_Pin GPIO_PIN_15
#define PWM_Trigger_GPIO_Port GPIOA
#define ADC_Trigger1_Pin GPIO_PIN_9
#define ADC_Trigger1_GPIO_Port GPIOB
#define Encoder_Data_Pin GPIO_PIN_5
#define Encoder_Data_GPIO_Port GPIOB
#define Motor_PWM_Pin GPIO_PIN_6
#define Motor_PWM_GPIO_Port GPIOB
#define DEL_2_Pin GPIO_PIN_14
#define DEL_2_GPIO_Port GPIOG
#define Clutch3_PWM_A_Pin GPIO_PIN_10
#define Clutch3_PWM_A_GPIO_Port GPIOA
#define Clutch2_PWM_A_Pin GPIO_PIN_9
#define Clutch2_PWM_A_GPIO_Port GPIOA
#define DEL_1_Pin GPIO_PIN_9
#define DEL_1_GPIO_Port GPIOG
#define ADC_Current2_Pin GPIO_PIN_7
#define ADC_Current2_GPIO_Port GPIOF
#define ADC_Current1_Pin GPIO_PIN_6
#define ADC_Current1_GPIO_Port GPIOF
#define Motor_Enable_Pin GPIO_PIN_8
#define Motor_Enable_GPIO_Port GPIOC
#define Motor_Error_Pin GPIO_PIN_7
#define Motor_Error_GPIO_Port GPIOC
#define ADC_Current3_Pin GPIO_PIN_8
#define ADC_Current3_GPIO_Port GPIOF
#define ADC_Pot1_Pin GPIO_PIN_0
#define ADC_Pot1_GPIO_Port GPIOA
#define ADC_Trigger3_Pin GPIO_PIN_2
#define ADC_Trigger3_GPIO_Port GPIOB
#define Clutch2_PWM_B_Pin GPIO_PIN_10
#define Clutch2_PWM_B_GPIO_Port GPIOE
#define Clutch3_PWM_B_Pin GPIO_PIN_12
#define Clutch3_PWM_B_GPIO_Port GPIOE
#define ADC_Pot2_Pin GPIO_PIN_1
#define ADC_Pot2_GPIO_Port GPIOA
#define Clutch1_PWM_A_Pin GPIO_PIN_9
#define Clutch1_PWM_A_GPIO_Port GPIOE
#define Encoder_CS_1_Pin GPIO_PIN_14
#define Encoder_CS_1_GPIO_Port GPIOD
#define Encoder_CS_2_Pin GPIO_PIN_15
#define Encoder_CS_2_GPIO_Port GPIOD
#define ADC_Pot3_Pin GPIO_PIN_2
#define ADC_Pot3_GPIO_Port GPIOA
#define ADC_Temp2_Pin GPIO_PIN_0
#define ADC_Temp2_GPIO_Port GPIOB
#define Encoder_CS_3_Pin GPIO_PIN_12
#define Encoder_CS_3_GPIO_Port GPIOF
#define SWRUN_Pin GPIO_PIN_15
#define SWRUN_GPIO_Port GPIOF
#define Clutch1_PWM_B_Pin GPIO_PIN_8
#define Clutch1_PWM_B_GPIO_Port GPIOE
#define ADC_Motor_Speed_Pin GPIO_PIN_3
#define ADC_Motor_Speed_GPIO_Port GPIOA
#define ADC_Temp1_Pin GPIO_PIN_7
#define ADC_Temp1_GPIO_Port GPIOA
#define ADC_Temp3_Pin GPIO_PIN_1
#define ADC_Temp3_GPIO_Port GPIOB
#define ADC_Trigger2_Pin GPIO_PIN_10
#define ADC_Trigger2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
