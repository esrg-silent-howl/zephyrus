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
#include "stm32f7xx_hal.h"

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
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define USER_Btn_EXTI_IRQn EXTI15_10_IRQn
#define NRF1_SCK_Pin GPIO_PIN_5
#define NRF1_SCK_GPIO_Port GPIOA
#define NRF1_MISO_Pin GPIO_PIN_6
#define NRF1_MISO_GPIO_Port GPIOA
#define NRF1_MOSI_Pin GPIO_PIN_7
#define NRF1_MOSI_GPIO_Port GPIOA
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define NRF1_INT_Pin GPIO_PIN_12
#define NRF1_INT_GPIO_Port GPIOF
#define NRF1_INT_EXTI_IRQn EXTI15_10_IRQn
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define NRF1_CSN_Pin GPIO_PIN_14
#define NRF1_CSN_GPIO_Port GPIOD
#define NRF1_CE_Pin GPIO_PIN_15
#define NRF1_CE_GPIO_Port GPIOD
#define NRF3_CE_Pin GPIO_PIN_2
#define NRF3_CE_GPIO_Port GPIOG
#define NRF3_INT_Pin GPIO_PIN_3
#define NRF3_INT_GPIO_Port GPIOG
#define NRF3_INT_EXTI_IRQn EXTI3_IRQn
#define NRF3_SCK_Pin GPIO_PIN_10
#define NRF3_SCK_GPIO_Port GPIOC
#define NRF3_MISO_Pin GPIO_PIN_11
#define NRF3_MISO_GPIO_Port GPIOC
#define NRF3_MOSI_Pin GPIO_PIN_12
#define NRF3_MOSI_GPIO_Port GPIOC
#define NRF3_CSN_Pin GPIO_PIN_2
#define NRF3_CSN_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
