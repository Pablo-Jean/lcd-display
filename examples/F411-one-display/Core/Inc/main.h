/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#define LCD1_D7_Pin GPIO_PIN_1
#define LCD1_D7_GPIO_Port GPIOA
#define LCD1_D6_Pin GPIO_PIN_2
#define LCD1_D6_GPIO_Port GPIOA
#define LCD1_D5_Pin GPIO_PIN_3
#define LCD1_D5_GPIO_Port GPIOA
#define LCD1_D4_Pin GPIO_PIN_4
#define LCD1_D4_GPIO_Port GPIOA
#define LCD1_E_Pin GPIO_PIN_5
#define LCD1_E_GPIO_Port GPIOA
#define LCD1_RS_Pin GPIO_PIN_6
#define LCD1_RS_GPIO_Port GPIOA
#define LCD2_RS_Pin GPIO_PIN_8
#define LCD2_RS_GPIO_Port GPIOA
#define LCD2_E_Pin GPIO_PIN_9
#define LCD2_E_GPIO_Port GPIOA
#define LCD2_D4_Pin GPIO_PIN_10
#define LCD2_D4_GPIO_Port GPIOA
#define LCD2_D5_Pin GPIO_PIN_11
#define LCD2_D5_GPIO_Port GPIOA
#define LCD2_D6_Pin GPIO_PIN_12
#define LCD2_D6_GPIO_Port GPIOA
#define LCD2_D7_Pin GPIO_PIN_15
#define LCD2_D7_GPIO_Port GPIOA
#define LCD2_BL_Pin GPIO_PIN_6
#define LCD2_BL_GPIO_Port GPIOB
#define LCD1_BL_Pin GPIO_PIN_8
#define LCD1_BL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
