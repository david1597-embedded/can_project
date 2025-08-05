/*
 * steeringservo.c
 *
 *  Created on: May 23, 2025
 *      Author: USER
 */

#include "steeringservo.h"

void steering_task(uint16_t steering_servo_ccr)
{
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, steering_servo_ccr);
  HAL_Delay(10);
}
