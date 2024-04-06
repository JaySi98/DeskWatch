/*
 * ssd1306.h
 *
 *  Created on: Mar 26, 2024
 *      Author: kuba
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

extern I2C_HandleTypeDef* ssd1306_i2c;

// I2C timeout
#define SSD1306_TIMEOUT 100
// display height in pixels
#define SSD1306_HEIGHT   64
// display width in pixels
#define SSD1306_WIDTH   128

// screen colors
typedef enum {
    Black = 0x00,
    White = 0x01
} SSD1306_Color;


// driver structure
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Initialized;
    uint8_t DisplayOn;
} SSD1306_HandleTypeDef;

typedef struct {
    uint8_t x;
    uint8_t y;
} SSD1306_Vertex;

HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c);
void SSD1306_Init(void);
void SSD1306_Fill(SSD1306_COLOR color);
void SSD1306_UpdateScreen(void);
void SSD1306_SetContrast(const uint8_t value);
void SSD1306_SetDisplayOn(const uint8_t on);
uint8_t SSD1306_GetDisplayOn();

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_Color color);
//char SSD1306_WriteChar(char ch, FontDef Font, SSD1306_Color color);
//char SSD1306_WriteString(char* str, FontDef Font, SSD1306_Color color);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color);
void SSD1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_Color color);
void SSD1306_DrawArcWithRadiusLine(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_Color color);
void SSD1306_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1306_Color color);
void SSD1306_FillCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r,SSD1306_Color par_color);
void SSD1306_Polyline(const SSD1306_Vertex *par_vertex, uint16_t par_size, SSD1306_Color color);
void SSD1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color);
void SSD1306_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color);
void SSD1306_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1306_Color color);

#ifdef __cplusplus
}
#endif

#endif /* INC_SSD1306_H_ */
