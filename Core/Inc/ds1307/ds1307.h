/*
 * ds1307.h
 *
 *  Created on: Mar 24, 2024
 *      Author: kuba
 */

#ifndef INC_DS1307_H_
#define INC_DS1307_H_

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

extern I2C_HandleTypeDef* ds1307_i2c;

// I2C timeout
#define DS1307_TIMEOUT		100

typedef enum DS1307_SQW_Rate
{
	DS1307_1HZ,
	DS1307_4096HZ,
	DS1307_8192HZ,
	DS1307_32768HZ
} DS1307_SQW_Rate;

typedef enum DS1307_SQW_Enable
{
	DS1307_SQW_DISABLED,
	DS1307_SQW_ENABLED
} DS1307_SQW_Enable;

void DS1307_Init(I2C_HandleTypeDef *hi2c);

void DS1307_Set_ClockHalt(uint8_t halt);
void DS1307_Set_EnableSquareWave(DS1307_SQW_Enable mode);
void DS1307_Set_InterruptRate(DS1307_SQW_Rate rate);

void DS1307_Set_DayOfWeek(uint8_t dow);
void DS1307_Set_Date(uint8_t date);
void DS1307_Set_Month(uint8_t month);
void DS1307_Set_Year(uint16_t year);
void DS1307_Set_Hour(uint8_t hour_24mode);
void DS1307_Set_Minute(uint8_t minute);
void DS1307_Set_Second(uint8_t second);
void DS1307_Set_TimeZone(int8_t hr, uint8_t min);

uint8_t  DS1307_Get_ClockHalt(void);
uint8_t  DS1307_Get_DayOfWeek(void);
uint8_t  DS1307_Get_Date(void);
uint8_t  DS1307_Get_Month(void);
uint16_t DS1307_Get_Year(void);
uint8_t  DS1307_Get_Hour(void);
uint8_t  DS1307_Get_Minute(void);
uint8_t  DS1307_Get_Second(void);
int8_t   DS1307_Get_TimeZoneHour(void);
uint8_t  DS1307_Get_TimeZoneMin(void);

HAL_StatusTypeDef DS1307_Write_RAM(uint8_t addr, const void* data, uint32_t size);
HAL_StatusTypeDef DS1307_Read_RAM(uint8_t addr, void* data, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif /* INC_DS1307_H_ */
