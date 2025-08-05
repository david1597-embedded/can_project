/*
 * com.h
 *
 *  Created on: May 26, 2025
 *      Author: USER
 */

#ifndef INC_COM_H_
#define INC_COM_H_

#include "main.h"
#include "can.h"

void com_init();
void sendMessage(int16_t * car_steering_angle, float * car_rpm, float * car_speed, uint8_t * car_direction, uint8_t * back_warn);
#endif /* INC_COM_H_ */
