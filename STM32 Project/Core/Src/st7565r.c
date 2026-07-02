/*
 * st7565r.c
 *
 *  Created on: Dec 21, 2023
 *      Author: Nyaaaa13
 */

#include "st7565r.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>  // For memcpy

#if defined(ST7565R_USE_SPI)

void st7565r_Reset(void) {
    // CS = High (not selected)
    HAL_GPIO_WritePin(ST7565R_CS_Port, ST7565R_CS_Pin, GPIO_PIN_SET);

    // Reset the OLED
    HAL_GPIO_WritePin(ST7565R_Reset_Port, ST7565R_Reset_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(ST7565R_Reset_Port, ST7565R_Reset_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
}

// Send a byte to the command register
void st7565r_WriteCommand(uint8_t byte) {
    HAL_GPIO_WritePin(ST7565R_CS_Port, ST7565R_CS_Pin, GPIO_PIN_RESET); // select OLED
    HAL_GPIO_WritePin(ST7565R_DC_Port, ST7565R_DC_Pin, GPIO_PIN_RESET); // command
    
    // Software SPI Bit-Bang
    for (uint8_t n = 0; n < 8; n++) {
        HAL_GPIO_WritePin(LCD_SI_GPIO_Port, LCD_SI_Pin, (byte & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        byte <<= 1;
        HAL_GPIO_WritePin(LCD_SC_GPIO_Port, LCD_SC_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LCD_SC_GPIO_Port, LCD_SC_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LCD_SC_GPIO_Port, LCD_SC_Pin, GPIO_PIN_RESET);
    }

    HAL_GPIO_WritePin(ST7565R_CS_Port, ST7565R_CS_Pin, GPIO_PIN_SET); // un-select OLED
}

// Send data
void st7565r_WriteData(uint8_t* buffer, size_t buff_size) {
    HAL_GPIO_WritePin(ST7565R_CS_Port, ST7565R_CS_Pin, GPIO_PIN_RESET); // select OLED
    HAL_GPIO_WritePin(ST7565R_DC_Port, ST7565R_DC_Pin, GPIO_PIN_SET); // data
    
    // Software SPI Bit-Bang
    for (size_t i = 0; i < buff_size; i++) {
        uint8_t byte = buffer[i];
        for (uint8_t n = 0; n < 8; n++) {
            HAL_GPIO_WritePin(LCD_SI_GPIO_Port, LCD_SI_Pin, (byte & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            byte <<= 1;
            HAL_GPIO_WritePin(LCD_SC_GPIO_Port, LCD_SC_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LCD_SC_GPIO_Port, LCD_SC_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LCD_SC_GPIO_Port, LCD_SC_Pin, GPIO_PIN_RESET);
        }
    }

    HAL_GPIO_WritePin(ST7565R_CS_Port, ST7565R_CS_Pin, GPIO_PIN_SET); // un-select OLED
}

void st7565r_DC_Select (uint8_t data) {
	if(!data) { HAL_GPIO_WritePin(ST7565R_DC_Port, ST7565R_DC_Pin, GPIO_PIN_RESET);}
	else { HAL_GPIO_WritePin(ST7565R_DC_Port, ST7565R_DC_Pin, GPIO_PIN_SET);}
	HAL_Delay(1);
}

#else
#error "You should define ST7565R_USE_SPI macro"
#endif


// Screenbuffer
static uint8_t ST7565R_Buffer[ST7565R_BUFFER_SIZE];

// Screen object
static ST7565R_t ST7565R;

/* Fills the Screenbuffer with values from a given buffer of a fixed length */
ST7565R_Error_t st7565r_FillBuffer(uint8_t* buf, uint32_t len) {
	ST7565R_Error_t ret = ST7565R_ERR;
    if (len <= ST7565R_BUFFER_SIZE) {
        memcpy(ST7565R_Buffer,buf,len);
        ret = ST7565R_OK;
    }
    return ret;
}

/* Initialize the oled screen */
void st7565r_Init(void) {
    // Reset OLED
	st7565r_Reset();

	// Reset DC pin
	st7565r_DC_Select(0);

    // Init LCD
    st7565r_WriteCommand(0xA1); // ADC select (Reverse)
    st7565r_WriteCommand(0xAE); // Display OFF
    st7565r_WriteCommand(0xC0); // COM direction scan (Reverse)
    st7565r_WriteCommand(0xA2); // LCD bias set
    st7565r_WriteCommand(0x2F); // Power Control set
    st7565r_WriteCommand(0x21); // Resistor Ratio Set
    st7565r_WriteCommand(0x81); // Electronic Volume Command (contrast)
    st7565r_WriteCommand(0x20); // Volume value
    st7565r_WriteCommand(0x40); // Display Start Line Set 0
    st7565r_WriteCommand(0xAF); // Display ON

    st7565r_DisplayClear();

/*
    // Init OLED
    st7565r_SetDisplayOn(0); //display off

    // Wait for the screen to boot
//    HAL_Delay(5);

    //Reset pin - H
//    HAL_GPIO_WritePin(ST7565R_CS_Port, ST7565R_CS_Pin, GPIO_PIN_RESET);

    //Wait 1usec
    HAL_Delay(1);

    //Initalization functions

    st7565r_WriteCommand(0x20); //Set Memory Addressing Mode
    st7565r_WriteCommand(0x00); // 00b,Horizontal Addressing Mode; 01b,Vertical Addressing Mode;
                                // 10b,Page Addressing Mode (RESET); 11b,Invalid

    st7565r_WriteCommand(0xB0); //Set Page Start Address for Page Addressing Mode,0-7

#ifdef ST7565R_MIRROR_VERT
    st7565r_WriteCommand(0xC0); // Mirror vertically
#else
    st7565r_WriteCommand(0xC8); //Set COM Output Scan Direction
#endif

    st7565r_WriteCommand(0x00); //---set low column address
    st7565r_WriteCommand(0x10); //---set high column address

    st7565r_WriteCommand(0x40); //--set start line address - CHECK

    st7565r_SetContrast(0xFF);

#ifdef ST7565R_MIRROR_HORIZ
    st7565r_WriteCommand(0xA0); // Mirror horizontally
#else
    st7565r_WriteCommand(0xA1); //--set segment re-map 0 to 127 - CHECK
#endif

#ifdef ST7565R_INVERSE_COLOR
    st7565r_WriteCommand(0xA7); //--set inverse color
#else
    st7565r_WriteCommand(0xA6); //--set normal color
#endif

// Set multiplex ratio.
#if (ST7565R_HEIGHT == 128)
    // Found in the Luma Python lib for SH1106.
    st7565r_WriteCommand(0xFF);
#else
    st7565r_WriteCommand(0xA8); //--set multiplex ratio(1 to 64) - CHECK
#endif

#if (ST7565R_HEIGHT == 32)
    st7565r_WriteCommand(0x1F); //
#elif (ST7565R_HEIGHT == 64)
    st7565r_WriteCommand(0x3F); //
#elif (ST7565R_HEIGHT == 128)
    st7565r_WriteCommand(0x3F); // Seems to work for 128px high displays too.
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    st7565r_WriteCommand(0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content

    st7565r_WriteCommand(0xD3); //-set display offset - CHECK
    st7565r_WriteCommand(0x00); //-not offset

    st7565r_WriteCommand(0xD5); //--set display clock divide ratio/oscillator frequency
    st7565r_WriteCommand(0xF0); //--set divide ratio

    st7565r_WriteCommand(0xD9); //--set pre-charge period
    st7565r_WriteCommand(0x22); //

    st7565r_WriteCommand(0xDA); //--set com pins hardware configuration - CHECK
#if (ST7565R_HEIGHT == 32)
    st7565r_WriteCommand(0x02);
#elif (ST7565R_HEIGHT == 64)
    st7565r_WriteCommand(0x12);
#elif (ST7565R_HEIGHT == 128)
    st7565r_WriteCommand(0x12);
#else
#error "Only 32, 64, or 128 lines of height are supported!"
#endif

    st7565r_WriteCommand(0xDB); //--set vcomh
    st7565r_WriteCommand(0x20); //0x20,0.77xVcc

    st7565r_WriteCommand(0x8D); //--set DC-DC enable
    st7565r_WriteCommand(0x14); //
    st7565r_SetDisplayOn(1); //--turn on ST7565R panel

    // Clear screen
    st7565r_Fill(White);

    // Flush buffer to screen
    st7565r_UpdateScreen();
*/
    // Set default values for screen object
    ST7565R.CurrentX = 0;
    ST7565R.CurrentY = 0;

    ST7565R.Initialized = 1;
}

/* Fill the whole screen with the given color */
void st7565r_Fill(ST7565R_COLOR color) {
    memset(ST7565R_Buffer, (color == Black) ? 0x00 : 0xFF, sizeof(ST7565R_Buffer));
}

void st7565r_DisplayClear(void){
 //unsigned char i, j;
 st7565r_Fill(Black);
 st7565r_UpdateScreen();
 HAL_Delay(5);
}

/* Write the screenbuffer with changed to the screen */
void st7565r_UpdateScreen(void) {
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    for(uint8_t i = 0; i < ST7565R_HEIGHT/8; i++) {
    	st7565r_WriteCommand(0xB0 + i); // Set the current RAM page address.
    	st7565r_WriteCommand(0x00 + ST7565R_X_OFFSET_LOWER);
    	st7565r_WriteCommand(0x10 + ST7565R_X_OFFSET_UPPER);
    	st7565r_WriteData(&ST7565R_Buffer[ST7565R_WIDTH*i],ST7565R_WIDTH);
    }
}

/*
 * Draw one pixel in the screenbuffer
 * X => X Coordinate
 * Y => Y Coordinate
 * color => Pixel color
 */
void st7565r_DrawPixel(uint8_t x, uint8_t y, ST7565R_COLOR color) {
    if(x >= ST7565R_WIDTH || y >= ST7565R_HEIGHT) {
        // Don't write outside the buffer
        return;
    }

    // Draw in the right color
    if(color == White) {
    	ST7565R_Buffer[x + (y / 8) * ST7565R_WIDTH] |= 1 << (y % 8);
    } else {
    	ST7565R_Buffer[x + (y / 8) * ST7565R_WIDTH] &= ~(1 << (y % 8));
    }
}

/*
 * Draw 1 char to the screen buffer
 * ch       => char om weg te schrijven
 * Font     => Font waarmee we gaan schrijven
 * color    => Black or White
 */

char st7565r_WriteChar(char ch, FontDef Font, ST7565R_COLOR color) {
    uint32_t i, b, j;

    // Check if character is valid
    if (ch < 32 || ch > 126)
        return 0;

    // Check remaining space on current line
    if (ST7565R_WIDTH < (ST7565R.CurrentX + Font.FontWidth) ||
    	ST7565R_HEIGHT < (ST7565R.CurrentY + Font.FontHeight))
    {
        // Not enough space on current line
        return 0;
    }

    // Use the font to write
    for(i = 0; i < Font.FontHeight; i++) {
        b = Font.data[(ch - 32) * Font.FontHeight + i];
        for(j = 0; j < Font.FontWidth; j++) {
            if((b << j) & 0x8000)  {
            	st7565r_DrawPixel(ST7565R.CurrentX + j, (ST7565R.CurrentY + i), (ST7565R_COLOR) color);
            } else {
            	st7565r_DrawPixel(ST7565R.CurrentX + j, (ST7565R.CurrentY + i), (ST7565R_COLOR)!color);
            }
        }
    }

    // The current space is now taken
    ST7565R.CurrentX += Font.FontWidth;

    // Return written char for validation
    return ch;
}

/* Write full string to screenbuffer */
char st7565r_WriteString(char* str, FontDef Font, ST7565R_COLOR color) {
    while (*str) {
        if (st7565r_WriteChar(*str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }
        str++;
    }

    // Everything ok
    return *str;
}

/* Position the cursor */
void st7565r_SetCursor(uint8_t x, uint8_t y) {
	ST7565R.CurrentX = x;
	ST7565R.CurrentY = y;
}

/* Draw line by Bresenhem's algorithm */
void st7565r_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ST7565R_COLOR color) {
    int32_t deltaX = abs(x2 - x1);
    int32_t deltaY = abs(y2 - y1);
    int32_t signX = ((x1 < x2) ? 1 : -1);
    int32_t signY = ((y1 < y2) ? 1 : -1);
    int32_t error = deltaX - deltaY;
    int32_t error2;

    st7565r_DrawPixel(x2, y2, color);

    while((x1 != x2) || (y1 != y2)) {
    	st7565r_DrawPixel(x1, y1, color);
        error2 = error * 2;
        if(error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }

        if(error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
    return;
}

/* Draw polyline */
void st7565r_Polyline(const ST7565R_VERTEX *par_vertex, uint16_t par_size, ST7565R_COLOR color) {
    uint16_t i;
    if(par_vertex == NULL) {
        return;
    }

    for(i = 1; i < par_size; i++) {
    	st7565r_Line(par_vertex[i - 1].x, par_vertex[i - 1].y, par_vertex[i].x, par_vertex[i].y, color);
    }

    return;
}

/* Convert Degrees to Radians */
static float st7565r_DegToRad(float par_deg) {
    return par_deg * 3.14 / 180.0;
}

/* Normalize degree to [0;360] */
static uint16_t st7565r_NormalizeTo0_360(uint16_t par_deg) {
    uint16_t loc_angle;
    if(par_deg <= 360) {
        loc_angle = par_deg;
    } else {
        loc_angle = par_deg % 360;
        loc_angle = ((par_deg != 0)?par_deg:360);
    }
    return loc_angle;
}

/*
 * DrawArc. Draw angle is beginning from 4 quart of trigonometric circle (3pi/2)
 * start_angle in degree
 * sweep in degree
 */
void st7565r_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, ST7565R_COLOR color) {
    static const uint8_t CIRCLE_APPROXIMATION_SEGMENTS = 36;
    float approx_degree;
    uint32_t approx_segments;
    uint8_t xp1,xp2;
    uint8_t yp1,yp2;
    uint32_t count = 0;
    uint32_t loc_sweep = 0;
    float rad;

    loc_sweep = st7565r_NormalizeTo0_360(sweep);

    count = (st7565r_NormalizeTo0_360(start_angle) * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_segments = (loc_sweep * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_degree = loc_sweep / (float)approx_segments;
    while(count < approx_segments)
    {
        rad = st7565r_DegToRad(count*approx_degree);
        xp1 = x + (int8_t)(sin(rad)*radius);
        yp1 = y + (int8_t)(cos(rad)*radius);
        count++;
        if(count != approx_segments) {
            rad = st7565r_DegToRad(count*approx_degree);
        } else {
            rad = st7565r_DegToRad(loc_sweep);
        }
        xp2 = x + (int8_t)(sin(rad)*radius);
        yp2 = y + (int8_t)(cos(rad)*radius);
        st7565r_Line(xp1,yp1,xp2,yp2,color);
    }

    return;
}

/*
 * Draw arc with radius line
 * Angle is beginning from 4 quart of trigonometric circle (3pi/2)
 * start_angle: start angle in degree
 * sweep: finish angle in degree
 */
void st7565r_DrawArcWithRadiusLine(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, ST7565R_COLOR color) {
    static const uint8_t CIRCLE_APPROXIMATION_SEGMENTS = 36;
    float approx_degree;
    uint32_t approx_segments;
    uint8_t xp1 = 0;
    uint8_t xp2 = 0;
    uint8_t yp1 = 0;
    uint8_t yp2 = 0;
    uint32_t count = 0;
    uint32_t loc_sweep = 0;
    float rad;

    loc_sweep = st7565r_NormalizeTo0_360(sweep);

    count = (st7565r_NormalizeTo0_360(start_angle) * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_segments = (loc_sweep * CIRCLE_APPROXIMATION_SEGMENTS) / 360;
    approx_degree = loc_sweep / (float)approx_segments;

    rad = st7565r_DegToRad(count*approx_degree);
    uint8_t first_point_x = x + (int8_t)(sin(rad)*radius);
    uint8_t first_point_y = y + (int8_t)(cos(rad)*radius);
    while (count < approx_segments) {
        rad = st7565r_DegToRad(count*approx_degree);
        xp1 = x + (int8_t)(sin(rad)*radius);
        yp1 = y + (int8_t)(cos(rad)*radius);
        count++;
        if (count != approx_segments) {
            rad = st7565r_DegToRad(count*approx_degree);
        } else {
            rad = st7565r_DegToRad(loc_sweep);
        }
        xp2 = x + (int8_t)(sin(rad)*radius);
        yp2 = y + (int8_t)(cos(rad)*radius);
        st7565r_Line(xp1,yp1,xp2,yp2,color);
    }

    // Radius line
    st7565r_Line(x,y,first_point_x,first_point_y,color);
    st7565r_Line(x,y,xp2,yp2,color);
    return;
}

/* Draw circle by Bresenhem's algorithm */
void st7565r_DrawCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r,ST7565R_COLOR par_color) {
    int32_t x = -par_r;
    int32_t y = 0;
    int32_t err = 2 - 2 * par_r;
    int32_t e2;

    if (par_x >= ST7565R_WIDTH || par_y >= ST7565R_HEIGHT) {
        return;
    }

    do {
    	st7565r_DrawPixel(par_x - x, par_y + y, par_color);
    	st7565r_DrawPixel(par_x + x, par_y + y, par_color);
    	st7565r_DrawPixel(par_x + x, par_y - y, par_color);
    	st7565r_DrawPixel(par_x - x, par_y - y, par_color);
        e2 = err;

        if (e2 <= y) {
            y++;
            err = err + (y * 2 + 1);
            if(-x == y && e2 <= x) {
                e2 = 0;
            }
        }

        if (e2 > x) {
            x++;
            err = err + (x * 2 + 1);
        }
    } while (x <= 0);

    return;
}

/* Draw filled circle. Pixel positions calculated using Bresenham's algorithm */
void st7565r_FillCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r,ST7565R_COLOR par_color) {
    int32_t x = -par_r;
    int32_t y = 0;
    int32_t err = 2 - 2 * par_r;
    int32_t e2;

    if (par_x >= ST7565R_WIDTH || par_y >= ST7565R_HEIGHT) {
        return;
    }

    do {
        for (uint8_t _y = (par_y + y); _y >= (par_y - y); _y--) {
            for (uint8_t _x = (par_x - x); _x >= (par_x + x); _x--) {
            	st7565r_DrawPixel(_x, _y, par_color);
            }
        }

        e2 = err;
        if (e2 <= y) {
            y++;
            err = err + (y * 2 + 1);
            if (-x == y && e2 <= x) {
                e2 = 0;
            }
        }

        if (e2 > x) {
            x++;
            err = err + (x * 2 + 1);
        }
    } while (x <= 0);

    return;
}

/* Draw a rectangle */
void st7565r_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ST7565R_COLOR color) {
	st7565r_Line(x1,y1,x2,y1,color);
	st7565r_Line(x2,y1,x2,y2,color);
	st7565r_Line(x2,y2,x1,y2,color);
	st7565r_Line(x1,y2,x1,y1,color);

    return;
}

/* Draw a filled rectangle */
void st7565r_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, ST7565R_COLOR color) {
    uint8_t x_start = ((x1<=x2) ? x1 : x2);
    uint8_t x_end   = ((x1<=x2) ? x2 : x1);
    uint8_t y_start = ((y1<=y2) ? y1 : y2);
    uint8_t y_end   = ((y1<=y2) ? y2 : y1);

    for (uint8_t y= y_start; (y<= y_end)&&(y<ST7565R_HEIGHT); y++) {
        for (uint8_t x= x_start; (x<= x_end)&&(x<ST7565R_WIDTH); x++) {
        	st7565r_DrawPixel(x, y, color);
        }
    }
    return;
}

/* Draw a bitmap */
void st7565r_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, ST7565R_COLOR color) {
    int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    if (x >= ST7565R_WIDTH || y >= ST7565R_HEIGHT) {
        return;
    }

    for (uint8_t j = 0; j < h; j++, y++) {
        for (uint8_t i = 0; i < w; i++) {
            if (i & 7) {
                byte <<= 1;
            } else {
                byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
            }

            if (byte & 0x80) {
            	st7565r_DrawPixel(x + i, y, color);
            }
        }
    }
    return;
}

void st7565r_SetContrast(const uint8_t value) {
    const uint8_t kSetContrastControlRegister = 0x81;
    st7565r_WriteCommand(kSetContrastControlRegister);
    st7565r_WriteCommand(value);
}

void st7565r_SetDisplayOn(const uint8_t on) {
    uint8_t value;
    if (on) {
        value = 0xAF;   // Display on
        ST7565R.DisplayOn = 1;
    } else {
        value = 0xAE;   // Display off
        ST7565R.DisplayOn = 0;
    }
    st7565r_WriteCommand(value);
}

uint8_t st7565r_GetDisplayOn() {
    return ST7565R.DisplayOn;
}
