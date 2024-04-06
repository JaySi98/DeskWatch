/*
 * ds1307.c
 *
 *  Created on: Mar 24, 2024
 *      Author: kuba
 */

#include "ds1307.h"

// I2C handler
I2C_HandleTypeDef* ds1307_i2c;

// DS1307 I2C address
#define DS1307_I2C_ADDR 	0x68

// DS1307 registers
#define DS1307_REG_SECOND 	0x00
#define DS1307_REG_MINUTE 	0x01
#define DS1307_REG_HOUR  	0x02
#define DS1307_REG_DOW    	0x03
#define DS1307_REG_DATE   	0x04
#define DS1307_REG_MONTH  	0x05
#define DS1307_REG_YEAR   	0x06
#define DS1307_REG_CONTROL 	0x07
#define DS1307_REG_UTC_HR	0x08
#define DS1307_REG_UTC_MIN	0x09
#define DS1307_REG_CENT    	0x10

// DS1307 RAM
#define DS1307_RAM_START   	0x08
#define DS1307_RAM_END   	0x3F


// static functions
// ----------------------------------------------------------------------------
static uint8_t decodeBCD(uint8_t val)
{
	return (((val & 0xf0) >> 4) * 10) + (val & 0x0f);
}

static uint8_t encodeBCD(uint8_t val)
{
	return (val % 10 + ((val / 10) << 4));
}

static void DS1307_SetRegByte(uint8_t regAddr, uint8_t val)
{
	uint8_t bytes[2] = { regAddr, val };
	HAL_I2C_Master_Transmit(ds1307_i2c, DS1307_I2C_ADDR << 1, bytes, 2, DS1307_TIMEOUT);
}

static uint8_t DS1307_GetRegByte(uint8_t regAddr)
{
	uint8_t val;
	HAL_I2C_Master_Transmit(ds1307_i2c, DS1307_I2C_ADDR << 1, &regAddr, 1, DS1307_TIMEOUT);
	HAL_I2C_Master_Receive(ds1307_i2c, DS1307_I2C_ADDR << 1, &val, 1, DS1307_TIMEOUT);
	return val;
}

// lib functions
// ----------------------------------------------------------------------------
void DS1307_Init(I2C_HandleTypeDef *hi2c)
{
	ds1307_i2c = hi2c;
	DS1307_Set_ClockHalt(0);
}

void DS1307_Set_ClockHalt(uint8_t halt)
{
	uint8_t ch = (halt ? 1 << 7 : 0);
	DS1307_SetRegByte(DS1307_REG_SECOND, ch | (DS1307_GetRegByte(DS1307_REG_SECOND) & 0x7f));
}


void DS1307_Set_EnableSquareWave(DS1307_SQW_Enable mode)
{
	uint8_t controlReg = DS1307_GetRegByte(DS1307_REG_CONTROL);
	uint8_t newControlReg = (controlReg & ~(1 << 4)) | ((mode & 1) << 4);
	DS1307_SetRegByte(DS1307_REG_CONTROL, newControlReg);
}

void DS1307_Set_InterruptRate(DS1307_SQW_Rate rate)
{
	uint8_t controlReg = DS1307_GetRegByte(DS1307_REG_CONTROL);
	uint8_t newControlReg = (controlReg & ~0x03) | rate;
	DS1307_SetRegByte(DS1307_REG_CONTROL, newControlReg);
}

void DS1307_Set_DayOfWeek(uint8_t dow)
{
	DS1307_SetRegByte(DS1307_REG_DOW, encodeBCD(dow));
}

void DS1307_Set_Date(uint8_t date)
{
	DS1307_SetRegByte(DS1307_REG_DATE, encodeBCD(date));
}

void DS1307_Set_Month(uint8_t month)
{
	DS1307_SetRegByte(DS1307_REG_MONTH, encodeBCD(month));
}

void DS1307_Set_Year(uint16_t year)
{
	DS1307_SetRegByte(DS1307_REG_CENT, year / 100);
	DS1307_SetRegByte(DS1307_REG_YEAR, encodeBCD(year % 100));
}

void DS1307_Set_Hour(uint8_t hour_24mode)
{
	DS1307_SetRegByte(DS1307_REG_HOUR, encodeBCD(hour_24mode & 0x3F));
}

void DS1307_Set_Minute(uint8_t minute)
{
	DS1307_SetRegByte(DS1307_REG_MINUTE, encodeBCD(minute));
}

void DS1307_Set_Second(uint8_t second)
{
	uint8_t ch = DS1307_Get_ClockHalt();
	DS1307_SetRegByte(DS1307_REG_SECOND, encodeBCD(second | ch));
}

void DS1307_Set_TimeZone(int8_t hr, uint8_t min)
{
	DS1307_SetRegByte(DS1307_REG_UTC_HR, hr);
	DS1307_SetRegByte(DS1307_REG_UTC_MIN, min);
}

uint8_t DS1307_Get_ClockHalt(void)
{
	return (DS1307_GetRegByte(DS1307_REG_SECOND) & 0x80) >> 7;
}

uint8_t DS1307_Get_DayOfWeek(void)
{
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_DOW));
}

uint8_t DS1307_Get_Date(void)
{
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_DATE));
}

uint8_t DS1307_Get_Month(void)
{
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_MONTH));
}

uint16_t DS1307_Get_Year(void)
{
	uint16_t cen = DS1307_GetRegByte(DS1307_REG_CENT) * 100;
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_YEAR)) + cen;
}

uint8_t DS1307_Get_Hour(void)
{
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_HOUR)  & 0x3F);
}

uint8_t DS1307_Get_Minute(void)
{
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_MINUTE));
}

uint8_t DS1307_Get_Second(void)
{
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_SECOND) & 0x7F);
}

int8_t DS1307_Get_TimeZoneHour(void)
{
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_UTC_HR));
}

uint8_t DS1307_Get_TimeZoneMin(void)
{
	return decodeBCD(DS1307_GetRegByte(DS1307_REG_UTC_MIN));
}


HAL_StatusTypeDef DS1307_Write_RAM(uint8_t addr, const void* data, uint32_t size)
{
	if(addr >= DS1307_RAM_START && addr <= DS1307_RAM_END)
	{
	    return HAL_I2C_Mem_Write(ds1307_i2c, DS1307_I2C_ADDR << 1, addr, 1, (void*)data, size, HAL_MAX_DELAY);
	}
	else
	{
		return HAL_ERROR;
	}
}

HAL_StatusTypeDef DS1307_Read_RAM(uint8_t addr, void* data, uint32_t size)
{
	if(addr >= DS1307_RAM_START && addr <= DS1307_RAM_END)
	{
		return HAL_I2C_Mem_Read(ds1307_i2c, DS1307_I2C_ADDR << 1, addr, 1, data, size, HAL_MAX_DELAY);
	}
	else
	{
		return HAL_ERROR;
	}
}

