/*
 * ssd1306_font.h
 *
 *  Created on: May 4, 2024
 *      Author: kuba
 */

#ifndef INC_SSD1306_FONT_H_
#define INC_SSD1306_FONT_H_

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26
#define SSD1306_INCLUDE_FONT_16x24

// font structure
typedef struct {
    const uint8_t Width;
    const uint8_t Height;
    const uint16_t* Data;
} SSD1306_Font;

#ifdef SSD1306_INCLUDE_FONT_6x8
extern SSD1306_Font Font_6x8;
#endif
#ifdef SSD1306_INCLUDE_FONT_7x10
extern SSD1306_Font Font_7x10;
#endif
#ifdef SSD1306_INCLUDE_FONT_11x18
extern SSD1306_Font Font_11x18;
#endif
#ifdef SSD1306_INCLUDE_FONT_16x26
extern SSD1306_Font Font_16x26;
#endif
#ifdef SSD1306_INCLUDE_FONT_16x24
extern SSD1306_Font Font_16x24;
#endif

#ifdef __cplusplus
}
#endif

#endif /* INC_SSD1306_FONT_H_ */
