/*
 * view_controler.h
 *
 *  Created on: May 22, 2024
 *      Author: kuba
 */

#ifndef INC_VIEW_CONTROLLER_VIEW_CONTROLLER_H_
#define INC_VIEW_CONTROLLER_VIEW_CONTROLLER_H_

#include "main.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum View
{
	View_main,
	View_raw,
	View_set_time,
	View_set_date,
} View;


typedef struct {
	View current_view;
} view_controller;

void change_view(View view);


#ifdef __cplusplus
}
#endif



#endif /* INC_VIEW_CONTROLLER_VIEW_CONTROLLER_H_ */
