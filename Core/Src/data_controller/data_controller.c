/*
 * data_controller.c
 *
 *  Created on: May 25, 2024
 *      Author: kuba
 */

#include "data_controller/data_controller.h"
#include "ds1307/ds1307.h"
#include "lps25hb/lps25hb.h"
#include "ds18b20/ds18b20.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#define LPS25HB_CONST 0.034162608734308
#define LPS25HB_KELVIN 273.15

static const char* DAYS_OF_WEEK[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
static data_controller controller;

HAL_StatusTypeDef dc_init(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef* timer_handle, uint8_t calibration, float height)
{
	HAL_StatusTypeDef result = HAL_OK;

	controller.handle_i2c = hi2c;
	controller.timer_handle = timer_handle;
	controller.LPS25HB_calibartion = calibration;
	controller.LPS25HB_height = height;
	controller.ds18b20_measure = 0;
	controller.ds18b20_temp = 0;

	DS1307_Init(hi2c);
	LPS25HB_Init(hi2c);
	LPS25HB_Set_Calib(calibration);
	if (ds18b20_init() != HAL_OK) {
		result = HAL_ERROR;
	}

	return result;
}

float dc_get_ds18b20_temp()
{
	// measure 1sec to get temp
	if(controller.ds18b20_measure == 0)
	{
		ds18b20_start_measure(NULL);
		HAL_TIM_Base_Start_IT(controller.timer_handle);
	}
	else
	{
		HAL_TIM_Base_Stop_IT(controller.timer_handle);
		float temp = ds18b20_get_temp(NULL);
		if(temp < 85.0)
		{
			controller.ds18b20_temp = temp;
		}

		controller.ds18b20_measure = 0;
	}

	return controller.ds18b20_temp;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == controller.timer_handle) {
	  controller.ds18b20_measure = 1;
  }
}

float dc_get_LPS25HB_temp()
{
	return LPS25HB_Get_Temp();
}

float dc_get_LPS25HB_relative_pressure()
{
	return LPS25HB_Get_Pressure();
}

float dc_get_LPS25HB_absolute_pressure()
{
	return (LPS25HB_Get_Pressure() * exp(LPS25HB_CONST * controller.LPS25HB_height / (LPS25HB_Get_Temp() + LPS25HB_KELVIN)));
}

void  dc_get_DS1307_date_time(char* buffer)
{
	uint8_t date   = DS1307_Get_Date();
	uint8_t month  = DS1307_Get_Month();
	uint16_t year  = DS1307_Get_Year();
	uint8_t dow    = DS1307_Get_DayOfWeek();
	uint8_t hour   = DS1307_Get_Hour();
	uint8_t minute = DS1307_Get_Minute();
	sprintf(buffer, "%04d-%02d-%02d %s %02d:%02d", year, month, date, DAYS_OF_WEEK[dow], hour, minute);
}
