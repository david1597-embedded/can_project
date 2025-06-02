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
void data_process(uint8_t *rxbuffer);
#endif /* INC_COM_H_ */
