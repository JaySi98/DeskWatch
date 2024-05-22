/*
 * lps25hb.h
 *
 *  Created on: Mar 25, 2024
 *      Author: kuba
 */

#ifndef INC_LPS25HB_H_
#define INC_LPS25HB_H_


#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

extern I2C_HandleTypeDef* lps25hb_i2c;

// I2C timeout
#define LPS25HB_TIMEOUT 100

HAL_StatusTypeDef LPS25HB_Init(I2C_HandleTypeDef *hi2c);

float LPS25HB_Get_Temp(void);
float LPS25HB_Get_Pressure(void);
void  LPS25HB_Set_Calib(uint16_t value);


#ifdef __cplusplus
}
#endif

#endif /* INC_LPS25HB_H_ */
