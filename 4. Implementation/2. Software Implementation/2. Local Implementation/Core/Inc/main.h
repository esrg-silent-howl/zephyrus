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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

typedef enum {	
	FALSE=0, TRUE
} bool;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* Check for GNU compiler */
#if defined (__GNUC__)
    /**/
    #ifndef _weak_ 
        #define _weak_      __attribute__((weak))
    #endif

    #ifndef _unused_ 
        #define _unused_      __attribute__((unused))
    #endif

    #ifndef _packed_ 
        #define _packed_      __attribute__((packed))
    #endif

#endif

/*!< Set __flag__  (1) */
#define F_SET(__flag__)			flags |= (uint32_t)(1U<<__flag__)
/*!< Clear __flag__ (0) */
#define F_CLEAR(__flag__)		flags &= (uint32_t)(~(1U<<__flag__))
/*!< Read __flag__ */
#define F_READ(__flag__)		((flags & (1<<__flag__)) >> __flag__)

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUTTON_DEBUG1_Pin GPIO_PIN_13
#define BUTTON_DEBUG1_GPIO_Port GPIOC
#define BUTTON_DEBUG1_EXTI_IRQn EXTI15_10_IRQn
#define BATTERY_LEVEL_Pin GPIO_PIN_4
#define BATTERY_LEVEL_GPIO_Port GPIOA
#define LED_DEBUG1_Pin GPIO_PIN_0
#define LED_DEBUG1_GPIO_Port GPIOB
#define USS_ECHO_FRONT_Pin GPIO_PIN_10
#define USS_ECHO_FRONT_GPIO_Port GPIOB
#define USS_ECHO_BACK_Pin GPIO_PIN_11
#define USS_ECHO_BACK_GPIO_Port GPIOB
#define MOTOR_R_DIR_Pin GPIO_PIN_13
#define MOTOR_R_DIR_GPIO_Port GPIOB
#define LED_DEBUG2_Pin GPIO_PIN_14
#define LED_DEBUG2_GPIO_Port GPIOB
#define MOTOR_L_DIR_Pin GPIO_PIN_15
#define MOTOR_L_DIR_GPIO_Port GPIOB
#define NRF24L01_CE_Pin GPIO_PIN_2
#define NRF24L01_CE_GPIO_Port GPIOG
#define NRF24L01_INT_Pin GPIO_PIN_3
#define NRF24L01_INT_GPIO_Port GPIOG
#define NRF24L01_INT_EXTI_IRQn EXTI3_IRQn
#define FLAG_DEBUG_Pin GPIO_PIN_6
#define FLAG_DEBUG_GPIO_Port GPIOC
#define MOTOR_R_PWM_Pin GPIO_PIN_8
#define MOTOR_R_PWM_GPIO_Port GPIOC
#define MOTOR_L_PWM_Pin GPIO_PIN_9
#define MOTOR_L_PWM_GPIO_Port GPIOC
#define NRF24L01_SCK_Pin GPIO_PIN_10
#define NRF24L01_SCK_GPIO_Port GPIOC
#define NRF24L01_MISO_Pin GPIO_PIN_11
#define NRF24L01_MISO_GPIO_Port GPIOC
#define NRF24L01_MOSI_Pin GPIO_PIN_12
#define NRF24L01_MOSI_GPIO_Port GPIOC
#define NRF24L01_CSN_Pin GPIO_PIN_2
#define NRF24L01_CSN_GPIO_Port GPIOD
#define USS_TRIGGER_Pin GPIO_PIN_3
#define USS_TRIGGER_GPIO_Port GPIOB
#define LED_POWER_Pin GPIO_PIN_7
#define LED_POWER_GPIO_Port GPIOB
#define MPU_6050_SCL_Pin GPIO_PIN_8
#define MPU_6050_SCL_GPIO_Port GPIOB
#define MPU_6050_SDA_Pin GPIO_PIN_9
#define MPU_6050_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
