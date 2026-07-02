/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ad9833.h"
#include "st7565r.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum Mode {
  MODE_FREQUENCY,
  MODE_PHASE
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define AD9833_FREQ_MAX 100000ULL // 100 kHz
#define AD9833_FREQ_MIN 1ULL      // 1 Hz
#define AD9833_FREQ_STEP 1000ULL  // 1 kHz
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t AD9833_Frequency = 1000; // 1 kHz
uint32_t AD9833_Phase = 0;        // 0 degrees
enum Mode AD9833_Mode = MODE_FREQUENCY;
AD9833_Waveform current_wave = AD9833_WAVEFORM_SINE;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void UpdateLCD(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  st7565r_Init();

  // Initialize AD9833 and generate a 1kHz sine wave
  AD9833_Init();
  AD9833_SetConfigF0P0(1000.0f, 0.0f, AD9833_WAVEFORM_SINE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    UpdateLCD();
    HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, AD_FSYNC_Pin|AD_SCLK_Pin|AD_SDAT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_RES_Pin|LCD_CS_Pin|LCD_SC_Pin|LCD_RS_Pin
                          |LCD_SI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : AD_FSYNC_Pin AD_SCLK_Pin AD_SDAT_Pin */
  GPIO_InitStruct.Pin = AD_FSYNC_Pin|AD_SCLK_Pin|AD_SDAT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ENC_CHA_Pin ENC_SW_Pin */
  GPIO_InitStruct.Pin = ENC_CHA_Pin|ENC_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ENC_CHB_Pin */
  GPIO_InitStruct.Pin = ENC_CHB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENC_CHB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RES_Pin LCD_CS_Pin LCD_SC_Pin LCD_RS_Pin
                           LCD_SI_Pin */
  GPIO_InitStruct.Pin = LCD_RES_Pin|LCD_CS_Pin|LCD_SC_Pin|LCD_RS_Pin
                          |LCD_SI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SW1_Pin SW2_Pin SW3_Pin SW4_Pin */
  GPIO_InitStruct.Pin = SW1_Pin|SW2_Pin|SW3_Pin|SW4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void UpdateLCD(void)
{
    char buf[32];
    st7565r_Fill(Black);
    
    // Waveform Type
    st7565r_SetCursor(0, 0);
    switch (current_wave) {
        case AD9833_WAVEFORM_SINE:        st7565r_WriteString("Wave: Sine", Font_7x10, White); break;
        case AD9833_WAVEFORM_TRIANGLE:    st7565r_WriteString("Wave: Triangle", Font_7x10, White); break;
        case AD9833_WAVEFORM_SQUARE:      st7565r_WriteString("Wave: Square", Font_7x10, White); break;
        case AD9833_WAVEFORM_SQUARE_HALF: st7565r_WriteString("Wave: Sq Half", Font_7x10, White); break;
    }

    // Frequency
    st7565r_SetCursor(0, 12);
    sprintf(buf, "Freq: %lu Hz %s", (unsigned long)AD9833_Frequency, (AD9833_Mode == MODE_FREQUENCY) ? "<" : "");
    st7565r_WriteString(buf, Font_7x10, White);

    // Phase
    st7565r_SetCursor(0, 24);
    sprintf(buf, "Phase: %lu deg %s", (unsigned long)AD9833_Phase, (AD9833_Mode == MODE_PHASE) ? "<" : "");
    st7565r_WriteString(buf, Font_7x10, White);

    st7565r_UpdateScreen();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last_interrupt_time = 0;
  uint32_t interrupt_time = HAL_GetTick();

  // debounce 50ms
  if (interrupt_time - last_interrupt_time > 50)
  {

    switch (GPIO_Pin)
    {
      case SW1_Pin:
        current_wave = AD9833_WAVEFORM_SINE;
        AD9833_SetWaveform(current_wave);
        break;
      case SW2_Pin:
        current_wave = AD9833_WAVEFORM_SQUARE;
        AD9833_SetWaveform(current_wave);
        break;
      case SW3_Pin:
        current_wave = AD9833_WAVEFORM_TRIANGLE;
        AD9833_SetWaveform(current_wave);
        break;
      case SW4_Pin:
        current_wave = AD9833_WAVEFORM_SQUARE_HALF;
        AD9833_SetWaveform(current_wave);
        break;
        
      case ENC_CHA_Pin: {
        static uint32_t last_enc_time = 0;
        if (interrupt_time - last_enc_time > 150) {
            last_enc_time = interrupt_time;
            if (AD9833_Mode == MODE_FREQUENCY) {
              // cw: channel signals are opposite values    
              if (HAL_GPIO_ReadPin(ENC_CHB_GPIO_Port, ENC_CHB_Pin) == GPIO_PIN_SET) { 
            // increase frequency
            if (AD9833_Frequency == AD9833_FREQ_MIN) {
              AD9833_Frequency = AD9833_FREQ_STEP; // Snap from 1Hz to 1000Hz
            } else if (AD9833_Frequency <= AD9833_FREQ_MAX - AD9833_FREQ_STEP) {
              AD9833_Frequency += AD9833_FREQ_STEP;
            } else {
              AD9833_Frequency = AD9833_FREQ_MAX;
            }
          // ccw: channel signals are same values
          } else {
            // decrease frequency
            if (AD9833_Frequency <= AD9833_FREQ_STEP) {
              AD9833_Frequency = AD9833_FREQ_MIN; // Snap down to 1Hz
            } else {
              AD9833_Frequency -= AD9833_FREQ_STEP;
            }
          }
          AD9833_SetFrequency_0((float)AD9833_Frequency);
        } 
        else if (AD9833_Mode == MODE_PHASE) {
          // cw: channel signals are opposite values    
          if (HAL_GPIO_ReadPin(ENC_CHB_GPIO_Port, ENC_CHB_Pin) == GPIO_PIN_SET) { 
            // increase phase 
            AD9833_Phase = (AD9833_Phase + 45) % 360;
          // ccw: channel signals are same values
          } else {
            // decrease phase
            if (AD9833_Phase < 45) {
              AD9833_Phase = 360 - (45 - AD9833_Phase);
            } else {
              AD9833_Phase -= 45;
            }
          }
          AD9833_SetPhase_0((float)AD9833_Phase);
        }
        } // end of encoder debounce
        break;
      }
      case ENC_SW_Pin:
        // Rotary encoder button pressed
        AD9833_Mode = !AD9833_Mode;
        AD9833_SetPhase_0((float)AD9833_Phase);
        break;
    }
  }
  last_interrupt_time = interrupt_time;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
