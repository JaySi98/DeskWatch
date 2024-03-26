/*
 * lps25hb.c
 *
 *  Created on: Mar 25, 2024
 *      Author: kuba
 */

#include "lps25hb.h"


// I2C handler
I2C_HandleTypeDef* lps25hb_i2c;

// LPS25HB I2C address
#define LPS25HB_ADDR			0xBA

// LPS25HB registers
#define LPS25HB_WHO_AM_I 		0x0F
#define LPS25HB_CTRL_REG1 		0x20
#define LPS25HB_CTRL_REG2 		0x21
#define LPS25HB_CTRL_REG3 		0x22
#define LPS25HB_CTRL_REG4 		0x23
#define LPS25HB_PRESS_OUT_XL 	0x28
#define LPS25HB_PRESS_OUT_L 	0x29
#define LPS25HB_PRESS_OUT_H 	0x2A
#define LPS25HB_TEMP_OUT_L 		0x2B
#define LPS25HB_TEMP_OUT_H 		0x2C
#define LPS25HB_RPDS_L 			0x39
#define LPS25HB_RPDS_H 			0x3A
#define LPS25HB_CTRL_REG1_PD 	0x80
#define LPS25HB_CTRL_REG1_ODR2 	0x40
#define LPS25HB_CTRL_REG1_ODR1 	0x20
#define LPS25HB_CTRL_REG1_ODR0 	0x10


// static functions
// ----------------------------------------------------------------------------
static uint8_t lps_read_reg(uint8_t reg)
{
    uint8_t value = 0;
    HAL_I2C_Mem_Read(lps25hb_i2c, LPS25HB_ADDR, reg, 1, &value, sizeof(value), LPS25HB_TIMEOUT);

    return value;
}

static void lps_write_reg(uint8_t reg, uint8_t value)
{
    HAL_I2C_Mem_Write(lps25hb_i2c, LPS25HB_ADDR, reg, 1, &value, sizeof(value), LPS25HB_TIMEOUT);
}


// lib functions
// ----------------------------------------------------------------------------
HAL_StatusTypeDef lps25hb_init(I2C_HandleTypeDef *hi2c)
{
	lps25hb_i2c = hi2c;
    if (lps_read_reg(LPS25HB_WHO_AM_I) != 0xBD)
         return HAL_ERROR;

    lps_write_reg(LPS25HB_CTRL_REG1,  0xC0);
    return HAL_OK;
}

float lps25hb_read_temp(void)
{
     int16_t temp;

     if (HAL_I2C_Mem_Read(lps25hb_i2c, LPS25HB_ADDR, LPS25HB_TEMP_OUT_L | 0x80, 1, (uint8_t*)&temp, sizeof(temp), LPS25HB_TIMEOUT) != HAL_OK)
         Error_Handler();

     return 42.5f + temp / 480.0f;
}

float lps25hb_read_pressure(void)
{
     int32_t pressure = 0;

     if (HAL_I2C_Mem_Read(lps25hb_i2c, LPS25HB_ADDR, LPS25HB_PRESS_OUT_XL | 0x80, 1, (uint8_t*)&pressure, 3, LPS25HB_TIMEOUT) != HAL_OK)
         Error_Handler();

     return pressure / 4096.0f;
}

void lps25hb_set_calib(uint16_t value)
{
	lps_write_reg(LPS25HB_RPDS_L, value);
	lps_write_reg(LPS25HB_RPDS_H, value >> 8);
}
