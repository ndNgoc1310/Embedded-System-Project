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
#include "stm32f1xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUTTON0_IN12_Pin GPIO_PIN_12
#define BUTTON0_IN12_GPIO_Port GPIOB
#define BUTTON0_IN12_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON1_IN13_Pin GPIO_PIN_13
#define BUTTON1_IN13_GPIO_Port GPIOB
#define BUTTON1_IN13_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON2_IN14_Pin GPIO_PIN_14
#define BUTTON2_IN14_GPIO_Port GPIOB
#define BUTTON2_IN14_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON3_IN15_Pin GPIO_PIN_15
#define BUTTON3_IN15_GPIO_Port GPIOB
#define BUTTON3_IN15_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON4_IN8_Pin GPIO_PIN_8
#define BUTTON4_IN8_GPIO_Port GPIOA
#define BUTTON4_IN8_EXTI_IRQn EXTI9_5_IRQn
#define RTC_IN5_Pin GPIO_PIN_5
#define RTC_IN5_GPIO_Port GPIOB
#define RTC_IN5_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
