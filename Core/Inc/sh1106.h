/*
 * sh1106.h
 *
 *  Created on: Apr 14, 2024
 *      Author: kuba
 */

#ifndef INC_SH1106_H_
#define INC_SH1106_H_

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

extern I2C_HandleTypeDef* sh1106_i2c;

// I2C timeout
#define SH1106_TIMEOUT 100
// display height in pixels
#define SH1106_HEIGHT   64
// display width in pixels
#define SH1106_WIDTH   128
// X offsets
#define SH1106_X_OFFSET_LOWER 0
#define SH1106_X_OFFSET_UPPER 0

// screen colors
typedef enum {
    Black = 0x00,
    White = 0x01
} SH1106_Color;

// driver structure
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Initialized;
    uint8_t DisplayOn;
} SH1106_HandleTypeDef;

typedef struct {
    uint8_t x;
    uint8_t y;
} SH1106_Vertex;

HAL_StatusTypeDef SH1106_Init(I2C_HandleTypeDef *hi2c);
void SH1106_UpdateScreen(void);
void SH1106_SetContrast(const uint8_t value);
void SH1106_SetDisplayOn(const uint8_t on);
uint8_t SH1106_GetDisplayOn();

void SH1106_DrawPixel(uint8_t x, uint8_t y, SH1106_Color color);
void SH1106_Fill(SH1106_Color color);

#ifdef __cplusplus
}
#endif

#endif /* INC_SH1106_H_ */
