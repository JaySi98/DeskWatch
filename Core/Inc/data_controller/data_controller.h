/*
 * data_controller.h
 *
 *  Created on: May 25, 2024
 *      Author: kuba
 */

#ifndef INC_DATA_CONTROLLER_DATA_CONTROLLER_H_
#define INC_DATA_CONTROLLER_DATA_CONTROLLER_H_

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
	I2C_HandleTypeDef* handle_i2c;
	TIM_HandleTypeDef* timer_handle;

	float LPS25HB_height;
	uint8_t LPS25HB_calibartion;

	uint8_t ds18b20_measure;
	float ds18b20_temp;

} data_controller;

HAL_StatusTypeDef dc_init(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef* timer_handle, uint8_t calibration, float height);
float dc_get_ds18b20_temp();
float dc_get_LPS25HB_temp();
float dc_get_LPS25HB_relative_pressure();
float dc_get_LPS25HB_absolute_pressure();
void  dc_get_DS1307_date_time(char* buffer);

#ifdef __cplusplus
}
#endif




#endif /* INC_DATA_CONTROLLER_DATA_CONTROLLER_H_ */
