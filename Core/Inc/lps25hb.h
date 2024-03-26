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

HAL_StatusTypeDef lps25hb_init(I2C_HandleTypeDef *hi2c);

float lps25hb_read_temp(void);
float lps25hb_read_pressure(void);
void lps25hb_set_calib(uint16_t value);


#ifdef __cplusplus
}
#endif

#endif /* INC_LPS25HB_H_ */
