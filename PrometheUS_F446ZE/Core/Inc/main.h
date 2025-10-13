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

extern SPI_HandleTypeDef hspi1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
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
#define Clutch3_PWM2_Pin GPIO_PIN_9
#define Clutch3_PWM2_GPIO_Port GPIOB
#define Encoder_Data_Pin GPIO_PIN_5
#define Encoder_Data_GPIO_Port GPIOB
#define Clutch3_PWM1_Pin GPIO_PIN_8
#define Clutch3_PWM1_GPIO_Port GPIOB
#define Clutch2_PWM1_Pin GPIO_PIN_6
#define Clutch2_PWM1_GPIO_Port GPIOB
#define DEL_2_Pin GPIO_PIN_14
#define DEL_2_GPIO_Port GPIOG
#define Clutch2_PWM2_Pin GPIO_PIN_7
#define Clutch2_PWM2_GPIO_Port GPIOB
#define Clutch1_PWM2_Pin GPIO_PIN_10
#define Clutch1_PWM2_GPIO_Port GPIOA
#define Clutch1_PWM1_Pin GPIO_PIN_9
#define Clutch1_PWM1_GPIO_Port GPIOA
#define DEL_1_Pin GPIO_PIN_9
#define DEL_1_GPIO_Port GPIOG
#define ADC_Current2_Pin GPIO_PIN_7
#define ADC_Current2_GPIO_Port GPIOF
#define ADC_Current1_Pin GPIO_PIN_6
#define ADC_Current1_GPIO_Port GPIOF
#define ADC_Current3_Pin GPIO_PIN_8
#define ADC_Current3_GPIO_Port GPIOF
#define ADC_Finger1_Pin GPIO_PIN_0
#define ADC_Finger1_GPIO_Port GPIOA
#define ADC_Trigger3_Pin GPIO_PIN_2
#define ADC_Trigger3_GPIO_Port GPIOB
#define ADC_Finger2_Pin GPIO_PIN_1
#define ADC_Finger2_GPIO_Port GPIOA
#define Encoder_CLK_Pin GPIO_PIN_5
#define Encoder_CLK_GPIO_Port GPIOA
#define Motor_PWM_Pin GPIO_PIN_9
#define Motor_PWM_GPIO_Port GPIOE
#define Encoder_CS_1_Pin GPIO_PIN_14
#define Encoder_CS_1_GPIO_Port GPIOD
#define Encoder_CS_2_Pin GPIO_PIN_15
#define Encoder_CS_2_GPIO_Port GPIOD
#define ADC_Finger3_Pin GPIO_PIN_2
#define ADC_Finger3_GPIO_Port GPIOA
#define ADC_Temp2_Pin GPIO_PIN_0
#define ADC_Temp2_GPIO_Port GPIOB
#define Encoder_CS_3_Pin GPIO_PIN_12
#define Encoder_CS_3_GPIO_Port GPIOF
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
