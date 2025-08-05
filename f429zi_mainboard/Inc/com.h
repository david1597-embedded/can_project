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
#include "usart.h"

void esp8266_send_data(uint8_t * data);
void com_init();
void data_process(uint8_t *rxbuffer);
void receive_data();
void can_error_process();
float * get_rpm_ptr(void);
float * get_speed_ptr(void);
uint8_t * get_can_state_flag_ptr(void);
uint8_t *get_back_warn_ptr();
#endif /* INC_COM_H_ */
