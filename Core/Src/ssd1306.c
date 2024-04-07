/*
 * ssd1306.c
 *
 *  Created on: Mar 26, 2024
 *      Author: kuba
 */


#include "ssd1306.h"
#include <string.h>  // For memcpy

// SSD1306 I2C address
#define SSD1306_ADDR (0x3C << 1)
#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8
#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

// SSD1306 commands
// Set Lower Column Start Address for Page Addressing Mode  0x00 - 0x0F
// Set Higher Column Start Address for Page Addressing Mode 0x10 - 0x1F
// Set Page Start Address for Page Addressing Mode          0xB0 - 0xB7
// Set Display Start Line                                   0x40 - 0x7F
// Set Segment Re-map                                       0xA0 - 0xA1

#define COMM_MEM_ADRR_MODE 	0x20 // Set Memory Addressing Mode
#define COMM_COL_ADDR  		0x21 // Set column Address mode
#define COMM_PAGE_ADDR 		0x22 // Set page Address mode
#define COMM_CONTRAST  		0x81 // Set Contrast for BANK0 0-255
#define COMM_ON_RESUME  	0xA4 // Entire Display ON - resume to RAM content
#define COMM_ON_FORCE   	0xA5 // Entire Display ON - forces regardless RAM
#define COMM_NORMAL_DISPL  	0xA6 // Set Display Inverse
#define COMM_INVERSE_DISPL 	0xA7 // Set Display Inverse
#define COMM_DISPL_ON  		0xAE // Set Display On
#define COMM_DISPL_OFF 		0xAF // Set Display Off
#define COMM_DISPL_OFFSET  	0xD3 // Set Display Offset
#define COMM_CLOCK_OSC		0xD5 // Set Display Clock Divide Ratio/ Oscillator Frequency
								 // Display Clock Divide Ratio 0xA0 - 0xA3
								 // Oscillator Frequenc 0xA4 - 0xA7
#define COMM_PRE_CHARGE_PER 0xD9 // Set Pre-charge Period
#define COMM_PINS_HARDWARE	0xDA // Set COM Pins Hardware Configuration
#define COMM_VCOMH_LEVEL	0xDB // Set VCOMH Deselect Level
#define COMM_NOP			0XE3 // No Operation Command

// Addressing modes
#define ADDR_PAGE 0x10
#define ADDR_HOR  0x00
#define ADDR_VER  0x01

// I2C handler
I2C_HandleTypeDef* ssd1306_i2c;
// Screen object
static SSD1306_HandleTypeDef ssd1306;
// Screenbuffer
static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];

// static functions
// ----------------------------------------------------------------------------

static HAL_StatusTypeDef SSD1306_WriteCommand(uint8_t byte)
{
	return HAL_I2C_Mem_Write(ssd1306_i2c, SSD1306_ADDR, SSD1306_COMMAND, 1, &byte, 1, SSD1306_TIMEOUT);
}

static HAL_StatusTypeDef ssd1306_WriteData(uint8_t* buffer, size_t buff_size)
{
	return HAL_I2C_Mem_Write(ssd1306_i2c, SSD1306_ADDR, SSD1306_DATA, 1, buffer, buff_size, SSD1306_TIMEOUT);
}

static HAL_StatusTypeDef ssd1306_FillBuffer(uint8_t* buf, uint32_t len)
{
	HAL_StatusTypeDef result = HAL_ERROR;

    if (len <= SSD1306_BUFFER_SIZE) {
        memcpy(SSD1306_Buffer,buf,len);
        result = HAL_OK;
    }

    return result;
}

// lib functions
// ----------------------------------------------------------------------------

HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c)
{
	ssd1306_i2c = hi2c;

	return HAL_OK;
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
	return 0;
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_Color color)
{

}

void SSD1306_Fill(SSD1306_Color color)
{

}

//char SSD1306_WriteChar(char ch, FontDef Font, SSD1306_Color color);
//char SSD1306_WriteString(char* str, FontDef Font, SSD1306_Color color);

//void SSD1306_SetCursor(uint8_t x, uint8_t y)
//{
//
//}
//
//void SSD1306_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color)
//{
//
//}
//
//void SSD1306_DrawArc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_Color color)
//{
//
//}
//
//void SSD1306_DrawArcWithRadiusLine(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, SSD1306_Color color)
//{
//
//}
//
//void SSD1306_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, SSD1306_Color color)
//{
//
//}
//
//void SSD1306_FillCircle(uint8_t par_x,uint8_t par_y,uint8_t par_r,SSD1306_Color par_color)
//{
//
//}
//
//void SSD1306_Polyline(const SSD1306_Vertex *par_vertex, uint16_t par_size, SSD1306_Color color)
//{
//
//}
//
//void SSD1306_DrawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color)
//{
//
//}
//
//void SSD1306_FillRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, SSD1306_Color color)
//{
//
//}
//
//void SSD1306_DrawBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, SSD1306_Color color)
//{
//
//}


