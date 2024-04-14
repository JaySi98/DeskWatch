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

static HAL_StatusTypeDef SSD1306_WriteData(uint8_t* buffer, size_t buff_size)
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

    // Wait for the screen to boot
	// propably usless
    HAL_Delay(100);

    // display off
    SSD1306_SetDisplayOn(0);

	// set horizontal addressing mode
	SSD1306_WriteCommand(COMM_MEM_ADRR_MODE);
	SSD1306_WriteCommand(ADDR_HOR);
	// set page start address for page addressing
//	SSD1306_WriteCommand(0XB0);
    // set com scan direction
    SSD1306_WriteCommand(0XC8);
	// set start line address
    SSD1306_WriteCommand(0X40);

    // set low and high column address
//	SSD1306_WriteCommand(0X04);
//	SSD1306_WriteCommand(0X1F);
	// set column address
	SSD1306_WriteCommand(0X21);
	SSD1306_WriteCommand(4);
	SSD1306_WriteCommand(10);

    // set contrast
    SSD1306_SetContrast(0x10);
    // Set segment re-map
    SSD1306_WriteCommand(0xA1);
    // set color
    SSD1306_WriteCommand(COMM_NORMAL_DISPL);
    // set multiplex ratio
    SSD1306_WriteCommand(0xA8);
//    SSD1306_WriteCommand(63);
    // set lcd height
	SSD1306_WriteCommand(0x3F);
    // resume to ram content
    SSD1306_WriteCommand(COMM_ON_RESUME);
    //set display offset
    SSD1306_WriteCommand(COMM_DISPL_OFFSET);
    SSD1306_WriteCommand(0x00);
    // set display clock - // set divide ratio
    SSD1306_WriteCommand(0xD5);
    SSD1306_WriteCommand(0xF0);
    // set pre-charge period
    SSD1306_WriteCommand(0xD9);
    SSD1306_WriteCommand(0x22);
    // set com pins hardware configuation
    SSD1306_WriteCommand(0xDA);
    SSD1306_WriteCommand(0x12);
    // set vcomh
    SSD1306_WriteCommand(0xDB);
    SSD1306_WriteCommand(0x20);
    // dc-dc enable
    SSD1306_WriteCommand(0x8D);
    SSD1306_WriteCommand(0 << 5 | 1 << 4 | 0x02);

    SSD1306_SetDisplayOn(1);
    SSD1306_Fill(Black);
    SSD1306_UpdateScreen();

    ssd1306.CurrentX = 0;
    ssd1306.CurrentY = 0;
    ssd1306.Initialized = 1;

	return HAL_OK;
}

void SSD1306_UpdateScreen(void)
{
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
    	SSD1306_WriteCommand(0xB0 + i); // Set the current RAM page address.
    	SSD1306_WriteCommand(0x00 + 2);
    	SSD1306_WriteCommand(0x10 + 0); //(2 >> 4)
//    	SSD1306_WriteCommand(2);
    	SSD1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

void SSD1306_SetContrast(const uint8_t value)
{
	SSD1306_WriteCommand(COMM_CONTRAST);
	SSD1306_WriteCommand(value);
}

void SSD1306_SetDisplayOn(const uint8_t on)
{
    uint8_t value;
    if (on) {
        value = 0xAF;   // Display on
        ssd1306.DisplayOn = 1;
    } else {
        value = 0xAE;   // Display off
        ssd1306.DisplayOn = 0;
    }
    SSD1306_WriteCommand(value);
}

uint8_t SSD1306_GetDisplayOn()
{
	return ssd1306.DisplayOn;
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_Color color)
{
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        // Don't write outside the buffer
        return;
    }

    // Draw in the right color
    if(color == White) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void SSD1306_Fill(SSD1306_Color color)
{
	memset(SSD1306_Buffer, (color == Black) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
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


