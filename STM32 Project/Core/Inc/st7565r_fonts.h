
#include <stdint.h>
#ifndef ST7565R_ST7565R_FONTS_H_
#define ST7565R_ST7565R_FONTS_H_

#include "st7565r_conf.h"

typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef;

#ifdef ST7565R_INCLUDE_FONT_6x8
extern FontDef Font_6x8;
#endif
#ifdef ST7565R_INCLUDE_FONT_7x10
extern FontDef Font_7x10;
#endif
#ifdef ST7565R_INCLUDE_FONT_11x18
extern FontDef Font_11x18;
#endif
#ifdef ST7565R_INCLUDE_FONT_16x26
extern FontDef Font_16x26;
#endif
#ifdef ST7565R_INCLUDE_FONT_16x24
extern FontDef Font_16x24;
#endif

#endif /* ST7565R_ST7565R_FONTS_H_ */
