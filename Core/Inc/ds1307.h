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

void DS1307_SetClockHalt(uint8_t halt);
void DS1307_SetEnableSquareWave(DS1307_SQW_Enable mode);
void DS1307_SetInterruptRate(DS1307_SQW_Rate rate);

void DS1307_SetDayOfWeek(uint8_t dow);
void DS1307_SetDate(uint8_t date);
void DS1307_SetMonth(uint8_t month);
void DS1307_SetYear(uint16_t year);
void DS1307_SetHour(uint8_t hour_24mode);
void DS1307_SetMinute(uint8_t minute);
void DS1307_SetSecond(uint8_t second);
void DS1307_SetTimeZone(int8_t hr, uint8_t min);

uint8_t  DS1307_GetClockHalt(void);
uint8_t  DS1307_GetDayOfWeek(void);
uint8_t  DS1307_GetDate(void);
uint8_t  DS1307_GetMonth(void);
uint16_t DS1307_GetYear(void);
uint8_t  DS1307_GetHour(void);
uint8_t  DS1307_GetMinute(void);
uint8_t  DS1307_GetSecond(void);
int8_t   DS1307_GetTimeZoneHour(void);
uint8_t  DS1307_GetTimeZoneMin(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_DS1307_H_ */
