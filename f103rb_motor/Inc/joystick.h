/*
 * joystick.h
 *
 *  Created on: May 21, 2025
 *      Author: USER
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "com.h"
#include "steeringservo.h"
#define STEERING_ADC_CENTER 2000
#define STEERING_ADC_DEADZONE 200
#define STEERING_MAX_ACCEL 100

/*for backward wheels*/
#define IN1_GPIO_PORT GPIOC
#define IN1_PIN       GPIO_PIN_7
#define IN2_GPIO_PORT GPIOA
#define IN2_PIN       GPIO_PIN_9
#define ENA_GPIO_PORT GPIOA
#define ENA_PIN       GPIO_PIN_15

/*for encoder motor*/
#define IN3_GPIO_PORT GPIOB
#define IN3_PIN       GPIO_PIN_1
#define IN4_GPIO_PORT GPIOB
#define IN4_PIN       GPIO_PIN_2
#define ENB_GPIO_PORT GPIOB
#define ENB_PIN       GPIO_PIN_10

/*factors related to calculate motor speed*/
#define PPR           7
#define RATIO         50
#define PI            3.14159265359
#define WHEEL_RADIUS  0.03

void joystick_init(void);
void joystick_task(void);
void adc_task(uint32_t adc_channel);
void motor_forward(void);
void motor_backward(void);
void cal_carspeed(void);
uint16_t * get_adcvalue_ptr(void);
uint8_t* get_back_warn_ptr(void);
uint8_t* get_joystick_task_flag_ptr(void);
uint8_t* get_conversion_complete_flag_ptr(void);

#endif /* INC_JOYSTICK_H_ */
