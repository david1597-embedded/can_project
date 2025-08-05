/*
 * ultrasonic.h
 *
 *  Created on: Jun 5, 2025
 *      Author: USER
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"
#include "tim.h"
#include "gpio.h"

typedef struct
{
  GPIO_TypeDef *port;     //ultrasonic sensor port
  uint16_t      pinNumber;//ultrasonic sensor pinNumber;
  uint8_t       capture_flag;
  uint16_t      distance;
}ULTRASONIC;

void ultra_module_Init();
void ultra_Init(ULTRASONIC *ultra, GPIO_TypeDef *port, uint16_t pinNumber);
void ultra_Trigger(ULTRASONIC *ultra, uint32_t TIM_IT_CCx );
void ultra_delay(uint16_t us);
ULTRASONIC * get_ultra_ptr();

#endif /* INC_ULTRASONIC_H_ */
