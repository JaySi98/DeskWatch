/*
 * sh1106.c
 *
 *  Created on: Apr 14, 2024
 *      Author: kuba
 */

#include "sh1106.h"
#include "string.h"

#define SH1106_ADDR 	(0x3C << 1) // 3D
#define SH1106_BUFFER_SIZE   SH1106_WIDTH * SH1106_HEIGHT / 8
#define SH1106_COMMAND 	0x00
#define SH1106_DATA 	0x40


// SH1106 commands
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


// I2C handler
I2C_HandleTypeDef* sh1106_i2c;
// Screen object
static SH1106_HandleTypeDef sh1106;
// Screenbuffer
static uint8_t SH1106_Buffer[SH1106_BUFFER_SIZE];

// static functions
// ----------------------------------------------------------------------------
static HAL_StatusTypeDef SH1106_WriteCommand(uint8_t byte)
{
	return HAL_I2C_Mem_Write(sh1106_i2c, SH1106_ADDR, SH1106_COMMAND, 1, &byte, 1, SH1106_TIMEOUT);
}

static HAL_StatusTypeDef SH1106_WriteData(uint8_t* buffer, size_t buff_size)
{
	return HAL_I2C_Mem_Write(sh1106_i2c, SH1106_ADDR, SH1106_DATA, 1, buffer, buff_size, SH1106_TIMEOUT);
}

// lib functions
// ----------------------------------------------------------------------------
HAL_StatusTypeDef SH1106_Init(I2C_HandleTypeDef *hi2c)
{
	sh1106_i2c = hi2c;

    // display off
//    SH1106_SetDisplayOn(0);
    SH1106_WriteCommand(0xAE);

    SH1106_WriteCommand(0xD5);
    SH1106_WriteCommand(0x80);
    SH1106_WriteCommand(0xA8);
    SH1106_WriteCommand(0x3F);
    SH1106_WriteCommand(0xD3);
    SH1106_WriteCommand(0x00);
    SH1106_WriteCommand(0x40);
    SH1106_WriteCommand(0xAD);
    SH1106_WriteCommand(0x8B);
    SH1106_WriteCommand(0xA1);
    SH1106_WriteCommand(0xC8);
    SH1106_WriteCommand(0xDA);
    SH1106_WriteCommand(0x12);
    SH1106_WriteCommand(0x81);
    SH1106_WriteCommand(0xFF);
    SH1106_WriteCommand(0xD9);
    SH1106_WriteCommand(0x1F);
    SH1106_WriteCommand(0xDB);
    SH1106_WriteCommand(0x40);
    SH1106_WriteCommand(0x33);
    SH1106_WriteCommand(0xA6);
    SH1106_WriteCommand(0x20);
    SH1106_WriteCommand(0x00);
    SH1106_WriteCommand(0xA4);

    // Wait for the screen to boot
    HAL_Delay(100);

    // set display on
    SH1106_WriteCommand(0xAF);

    SH1106_Fill(Black);
    SH1106_UpdateScreen();

    sh1106.CurrentX = 0;
    sh1106.CurrentY = 0;
    sh1106.Initialized = 1;

	return HAL_OK;
}

void SH1106_UpdateScreen(void)
{
    for(uint8_t i = 0; i < SH1106_HEIGHT/8; i++) {
    	SH1106_WriteCommand(0xB0 + i); // Set the current RAM page address.
    	SH1106_WriteCommand(0x00 + SH1106_X_OFFSET_LOWER);
    	SH1106_WriteCommand(0x10 + (2 >> 4)); //SH1106_X_OFFSET_UPPER
    	SH1106_WriteCommand(2 & 0xF);
    	SH1106_WriteData(&SH1106_Buffer[SH1106_WIDTH*i],SH1106_WIDTH);
    }
}

void SH1106_SetContrast(const uint8_t value)
{

}

void SH1106_SetDisplayOn(const uint8_t on)
{

}

uint8_t SH1106_GetDisplayOn()
{
	return 0;
}

void SH1106_DrawPixel(uint8_t x, uint8_t y, SH1106_Color color)
{
    if(x >= SH1106_WIDTH || y >= SH1106_HEIGHT) {
        // Don't write outside the buffer
        return;
    }

    // Draw in the right color
    if(color == White) {
    	SH1106_Buffer[x + (y / 8) * SH1106_WIDTH] |= 1 << (y % 8);
    } else {
    	SH1106_Buffer[x + (y / 8) * SH1106_WIDTH] &= ~(1 << (y % 8));
    }
}

void SH1106_Fill(SH1106_Color color)
{
	memset(SH1106_Buffer, (color == Black) ? 0x00 : 0xFF, sizeof(SH1106_Buffer));
}
