/*
 * com.c
 *
 *  Created on: May 26, 2025
 *      Author: USER
 */

#include "com.h"

static CAN_FilterTypeDef canFilter1;
static CAN_RxHeaderTypeDef canRxHeader;
static uint8_t RxData[8];

static int16_t steering_angle_tosend;
static float car_rpm_tosend;
static float car_speed_tosend;
static uint16_t car_direction_tosend;

static uint8_t data_process_complete = 0;
void com_init()
{
   /*Initializing canfilter*/
   canFilter1.FilterMaskIdHigh = 0x7f3 << 5;
   canFilter1.FilterIdHigh = 0x106 << 5;
   canFilter1.FilterMaskIdLow = 0x7f3 << 5;
   canFilter1.FilterIdLow = 0x106 << 5;
   canFilter1.FilterMode = CAN_FILTERMODE_IDMASK;
   canFilter1.FilterScale = CAN_FILTERSCALE_16BIT;
   canFilter1.FilterFIFOAssignment = CAN_FILTER_FIFO0;
   canFilter1.FilterBank = 0;
   canFilter1.FilterActivation = ENABLE;

   HAL_CAN_ConfigFilter(&hcan1, &canFilter1);
   HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
   HAL_CAN_Start(&hcan1);
   /*Initializing Esp8266*/
}

void data_process(uint8_t *rxbuffer)
{
   /*data processing*/
  steering_angle_tosend = ((int16_t)rxbuffer[0] << 8) | rxbuffer[1];
  car_rpm_tosend = ((uint16_t)rxbuffer[2] << 8) | rxbuffer[3];
  car_speed_tosend = ((uint16_t)rxbuffer[4] << 8) | rxbuffer[5];
  car_direction_tosend = ((uint16_t)rxbuffer[6] << 8) | rxbuffer[7];

  car_rpm_tosend = (float)(car_rpm_tosend / 100.0f);
  car_speed_tosend = (float)(car_speed_tosend / 1000.0f);
  HAL_Delay(1);
  data_process_complete = 0;
}

/*Data seperating when Received data from F103RB#1 Module(motor control)*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
     if(hcan->Instance == CAN1)
     {
         if(data_process_complete == 0)
         {
           data_process_complete = 1;
           HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHeader, &RxData[0]);
           data_process(&RxData[0]);
         }
     }
}
