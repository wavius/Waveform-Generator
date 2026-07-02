/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

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
#define AD_FSYNC_Pin GPIO_PIN_4
#define AD_FSYNC_GPIO_Port GPIOA
#define AD_SCLK_Pin GPIO_PIN_5
#define AD_SCLK_GPIO_Port GPIOA
#define AD_SDAT_Pin GPIO_PIN_7
#define AD_SDAT_GPIO_Port GPIOA
#define ENC_CHA_Pin GPIO_PIN_0
#define ENC_CHA_GPIO_Port GPIOB
#define ENC_CHA_EXTI_IRQn EXTI0_1_IRQn
#define ENC_CHB_Pin GPIO_PIN_1
#define ENC_CHB_GPIO_Port GPIOB
#define ENC_SW_Pin GPIO_PIN_2
#define ENC_SW_GPIO_Port GPIOB
#define ENC_SW_EXTI_IRQn EXTI2_3_IRQn
#define LCD_RES_Pin GPIO_PIN_11
#define LCD_RES_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOB
#define LCD_SC_Pin GPIO_PIN_13
#define LCD_SC_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_14
#define LCD_RS_GPIO_Port GPIOB
#define LCD_SI_Pin GPIO_PIN_15
#define LCD_SI_GPIO_Port GPIOB
#define SW1_Pin GPIO_PIN_8
#define SW1_GPIO_Port GPIOA
#define SW1_EXTI_IRQn EXTI4_15_IRQn
#define SW2_Pin GPIO_PIN_9
#define SW2_GPIO_Port GPIOA
#define SW2_EXTI_IRQn EXTI4_15_IRQn
#define SW3_Pin GPIO_PIN_10
#define SW3_GPIO_Port GPIOA
#define SW3_EXTI_IRQn EXTI4_15_IRQn
#define SW4_Pin GPIO_PIN_11
#define SW4_GPIO_Port GPIOA
#define SW4_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */

// Switch Function Assignments:
// SW1: SINE Wave
// SW2: SQUARE Wave
// SW3: TRIANGLE Wave
// SW4: SAWTOOTH Wave

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
