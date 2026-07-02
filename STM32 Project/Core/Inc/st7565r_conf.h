/**
 * Private configuration file for the ST7565R library.
 * This example is configured for STM32F0, I2C and including all fonts.
 */

#ifndef __ST7565R_CONF_H__
#define __ST7565R_CONF_H__
#include "main.h"
// Choose a microcontroller family
//#define STM32F0
//#define STM32F1
#define STM32F4
//#define STM32L0
//#define STM32L1
//#define STM32L4
//#define STM32F3
//#define STM32H7
//#define STM32F7
//#define STM32G0

// Choose a bus
#define ST7565R_USE_SPI

// SPI Configuration
#define ST7565R_SPI_PORT        hspi1
#define ST7565R_CS_Port         LCD_CS_GPIO_Port
#define ST7565R_CS_Pin          LCD_CS_Pin
#define ST7565R_DC_Port         LCD_RS_GPIO_Port
#define ST7565R_DC_Pin          LCD_RS_Pin
#define ST7565R_Reset_Port      LCD_RES_GPIO_Port
#define ST7565R_Reset_Pin       LCD_RES_Pin

// Mirror the screen if needed
#define ST7565R_MIRROR_VERT
//#define ST7565R_MIRROR_HORIZ

// Set inverse color if needed
// # define ST7565R_INVERSE_COLOR

// Include only needed fonts
#define ST7565R_INCLUDE_FONT_6x8
#define ST7565R_INCLUDE_FONT_7x10
#define ST7565R_INCLUDE_FONT_11x18
#define ST7565R_INCLUDE_FONT_16x26

#define ST7565R_INCLUDE_FONT_16x24

// The width of the screen can be set using this
// define. The default value is 128.
 #define ST7565R_WIDTH           132

// If your screen horizontal axis does not start
// in column 0 you can use this define to
// adjust the horizontal offset
#define ST7565R_X_OFFSET 4

// The height can be changed as well if necessary.
// It can be 32, 64 or 128. The default value is 64.
 #define ST7565R_HEIGHT          64

#endif /* __ST7565R_CONF_H__ */
