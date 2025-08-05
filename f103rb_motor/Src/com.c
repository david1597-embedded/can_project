/*
 * com.c
 *
 *  Created on: May 26, 2025
 *      Author: USER
 */


#include "com.h"

static CAN_TxHeaderTypeDef TxHeader;
static uint8_t TxData[8];
static uint32_t TxMailbox;
void com_init()
{
  /* Can Start */
   if (HAL_CAN_Start(&hcan) != HAL_OK)
   {
     /* Start Error */
     Error_Handler();
   }

}

void sendMessage(int16_t * car_steering_angle, float * car_rpm, float * car_speed, uint8_t * car_direction, uint8_t *back_warn)
{
   int16_t temp_data0 = *car_steering_angle;
   uint16_t temp_data1 = (uint16_t)(*car_rpm * 100.0);
   uint16_t temp_data2 = (uint16_t)(*car_speed * 1000.0);
   uint8_t temp_data3 = *car_direction;
   uint8_t temp_data4 = *back_warn;

   TxHeader.StdId = 0x102; // Message about controlling motor system
   TxHeader.ExtId = 0;
   TxHeader.RTR   = CAN_RTR_DATA;
   TxHeader.IDE   = CAN_ID_STD;
   TxHeader.DLC   = 8;
   TxHeader.TransmitGlobalTime = DISABLE;

   TxData[0] = (temp_data0 >> 8) & 0xFF;
   TxData[1] = temp_data0 & 0xFF;
   TxData[2] = (temp_data1>> 8) & 0xFF;
   TxData[3] = temp_data1 & 0xFF;
   TxData[4] = (temp_data2 >> 8) & 0xFF;
   TxData[5] = temp_data2 & 0xFF;
   TxData[6] = temp_data3;
   TxData[7] = temp_data4;

   TxMailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
   if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, &TxData[0], &TxMailbox) != HAL_OK)
   {
     Error_Handler();
   }

}
