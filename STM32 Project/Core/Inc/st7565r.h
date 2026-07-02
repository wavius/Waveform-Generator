/**
 * This library is written based on the library for OLED display on SSD1306, made by Olivier Van den Eede (4ilo) in 2016.
 * Based on refactoring from Aleksander Alekseev (afiskon) in 2018.
 * Refactoring done by Emil Gazizov (Nyaaangineer) in 2023.
 *
 * https://github.com/comming soon :)
 */

#ifndef __ST7565R_H__
#define __ST7565R_H__

#include <stddef.h>
#include <_ansi.h>

_BEGIN_STD_C

#include "st7565r_conf.h"


#if defined(STM32WB)
#include "stm32wbxx_hal.h"
#elif defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32L1)
#include "stm32l1xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32L5)
#include "stm32l5xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#else
#error "ST7565R library was tested only on STM32F0, STM32F1, STM32F3, STM32F4, STM32F7, STM32L0, STM32L1, STM32L4, STM32H7, STM32G0, STM32G4, STM32WB MCU families. Please modify st7565r.h if you know what you are doing. Also please send a pull request if it turns out the library works on other MCU's as well!"
#endif


#ifdef ST7565R_X_OFFSET
#define ST7565R_X_OFFSET_LOWER (ST7565R_X_OFFSET & 0x0F)
#define ST7565R_X_OFFSET_UPPER ((ST7565R_X_OFFSET >> 4) & 0x07)
#else
#define ST7565R_X_OFFSET_LOWER 0
#define ST7565R_X_OFFSET_UPPER 0
#endif

#include "st7565r_fonts.h"


/* vvv SPI config vvv */

#ifndef ST7565R_SPI_PORT
#define ST7565R_SPI_PORT        hspi2
#endif

#ifndef ST7565R_CS_Port
#define ST7565R_CS_Port         GPIOB
#endif
#ifndef ST7565R_CS_Pin
#define ST7565R_CS_Pin          GPIO_PIN_12
#endif

#ifndef ST7565R_DC_Port
#define ST7565R_DC_Port         GPIOB
#endif
#ifndef ST7565R_DC_Pin
#define ST7565R_DC_Pin          GPIO_PIN_14
#endif

#ifndef ST7565R_Reset_Port
#define ST7565R_Reset_Port      GPIOA
#endif
#ifndef ST7565R_Reset_Pin
#define ST7565R_Reset_Pin       GPIO_PIN_8
#endif

/* ^^^ SPI config ^^^ */

#if defined(ST7565R_USE_SPI)
// We are software bit-banging, so we don't need the SPI_HandleTypeDef.
#else
#error "You should define ST7565R_USE_SPI macro!"
#endif

void st7565r_DisplayClear(void);

// ST7565R display height in pixels
#ifndef ST7565R_HEIGHT
#define ST7565R_HEIGHT          64
#endif

// ST7565R width in pixels
#ifndef ST7565R_WIDTH
#define ST7565R_WIDTH           128
#endif

#ifndef ST7565R_BUFFER_SIZE
#define ST7565R_BUFFER_SIZE   ST7565R_WIDTH * ST7565R_HEIGHT / 8
#endif

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0xFF  // Pixel is set. Color depends on OLED
} ST7565R_COLOR;

typedef enum {
	ST7565R_OK = 0x00,
	ST7565R_ERR = 0x01  // Generic error.
} ST7565R_Error_t;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Initialized;
    uint8_t DisplayOn;
} ST7565R_t;

typedef struct {
    uint8_t x;
    uint8_t y;
} ST7565R_VERTEX;

// Procedure definitions
void st7565r_Init(void);
void st7565r_Fill(ST7565R_COLOR color);
void st7565r_UpdateScreen(void);
void st7565r_DrawPixel(uint8_t x, uint8_t y, ST7565R_COLOR color);
char st7565r_WriteChar(char ch, FontDef Font, ST7565R_COLOR color);
char st7565r_WriteString(char* str, FontDef Font, ST7565R_COLOR color);
void st7565r_SetCursor(uint8_t x, uint8_t y);
void st7565r_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ST7565R_COLOR color);
void st7565r_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, ST7565R_COLOR color);
void st7565r_DrawArcWithRadiusLine(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, ST7565R_COLOR color);
void st7565r_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, ST7565R_COLOR color);
void st7565r_FillCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r, ST7565R_COLOR par_color);
void st7565r_Polyline(const ST7565R_VERTEX *par_vertex, uint16_t par_size, ST7565R_COLOR color);
void st7565r_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ST7565R_COLOR color);
void st7565r_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ST7565R_COLOR color);
void st7565r_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, ST7565R_COLOR color);

/**
 * @brief Sets the contrast of the display.
 * @param[in] value contrast to set.
 * @note Contrast increases as the value increases.
 * @note RESET = 7Fh.
 */
void st7565r_SetContrast(const uint8_t value);

/**
 * @brief Set Display ON/OFF.
 * @param[in] on 0 for OFF, any for ON.
 */
void st7565r_SetDisplayOn(const uint8_t on);

/**
 * @brief Reads DisplayOn state.
 * @return  0: OFF.
 *          1: ON.
 */
uint8_t st7565r_GetDisplayOn();

// Low-level procedures
void st7565r_Reset(void);
void st7565r_WriteCommand(uint8_t byte);
void st7565r_WriteData(uint8_t* buffer, size_t buff_size);
ST7565R_Error_t st7565r_FillBuffer(uint8_t* buf, uint32_t len);

_END_STD_C

#endif // __ST7565R__
