/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define C0_In_Pin GPIO_PIN_7
#define C0_In_GPIO_Port GPIOE
#define C1_In_Pin GPIO_PIN_8
#define C1_In_GPIO_Port GPIOE
#define C2_In_Pin GPIO_PIN_9
#define C2_In_GPIO_Port GPIOE
#define C3_In_Pin GPIO_PIN_10
#define C3_In_GPIO_Port GPIOE
#define RS_Pin GPIO_PIN_6
#define RS_GPIO_Port GPIOC
#define EN_Pin GPIO_PIN_7
#define EN_GPIO_Port GPIOC
#define D4_Pin GPIO_PIN_0
#define D4_GPIO_Port GPIOD
#define D5_Pin GPIO_PIN_1
#define D5_GPIO_Port GPIOD
#define D6_Pin GPIO_PIN_2
#define D6_GPIO_Port GPIOD
#define D7_Pin GPIO_PIN_3
#define D7_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
