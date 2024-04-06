/*
 * ssd1306.c
 *
 *  Created on: Mar 26, 2024
 *      Author: kuba
 */

#include "ssd1306.h"

// I2C handler
I2C_HandleTypeDef* ssd1306_i2c;
// Screen object
static SSD1306_HandleTypeDef ssd1306;

// SSD1306 I2C address
#define SSD1306_ADDR (0x3C << 1)
#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8

// static functions
// ----------------------------------------------------------------------------

static void SSD1306_WriteCommand(uint8_t byte)
{

}

static void ssd1306_WriteData(uint8_t* buffer, size_t buff_size)
{

}

static HAL_StatusTypeDef ssd1306_FillBuffer(uint8_t* buf, uint32_t len)
{

}

// lib functions
// ----------------------------------------------------------------------------

HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c)
{

}

void SSD1306_Fill(SSD1306_COLOR color)
{

}

void SSD1306_UpdateScreen(void)
{

}

void SSD1306_SetContrast(const uint8_t value)
{

}

void SSD1306_SetDisplayOn(const uint8_t on)
{

}

uint8_t SSD1306_GetDisplayOn()
{

}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_Color color)
{

}

//char SSD1306_WriteChar(char ch, FontDef Font, SSD1306_Color color);
//char SSD1306_WriteString(char* str, FontDef Font, SSD1306_Color color);

void SSD1306_SetCursor(uint8_t x, uint8_t y)
{

}

void SSD1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color)
{

}

void SSD1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_Color color)
{

}

void SSD1306_DrawArcWithRadiusLine(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_Color color)
{

}

void SSD1306_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1306_Color color)
{

}

void SSD1306_FillCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r,SSD1306_Color par_color)
{

}

void SSD1306_Polyline(const SSD1306_Vertex *par_vertex, uint16_t par_size, SSD1306_Color color)
{

}

void SSD1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color)
{

}

void SSD1306_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color)
{

}

void SSD1306_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1306_Color color)
{

}


