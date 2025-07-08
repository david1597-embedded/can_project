/*
 * com.c
 *
 *  Created on: May 26, 2025
 *      Author: USER
 */

#include "com.h"
#include <stdio.h>
#include <string.h>
static CAN_FilterTypeDef canFilter1;
static CAN_RxHeaderTypeDef canRxHeader;
static uint8_t RxData[8];

static int16_t steering_angle_tosend;
static float car_rpm_tosend;
static float car_speed_tosend;
static uint8_t car_direction_tosend;
static uint8_t car_back_warn_tosend;
static uint8_t dashboard_can_state_flag = 0; // 0: noraml 1: Error Warning 2: Error passive
//static uint8_t data_process_complete = 0;

//Mutex Handle

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


//Error processing dashboard can state
void can_error_process()
{
  uint32_t error_code = HAL_CAN_GetError(&hcan1);
  uint32_t lec = (hcan1.Instance->ESR & CAN_ESR_LEC) >> CAN_ESR_LEC_Pos; // Last Error Code
  uint32_t rec = (hcan1.Instance->ESR & CAN_ESR_REC) >> CAN_ESR_REC_Pos; // Receive Error Counter
  uint32_t fovr = hcan1.Instance->RF0R & CAN_RF0R_FOVR0; // FIFO Overrun
  uint32_t boff = hcan1.Instance->MSR & CAN_ESR_BOFF; // Bus-Off
  uint32_t epvf = hcan1.Instance->ESR & CAN_ESR_EPVF; // Error Passive
  uint32_t ewgf = hcan1.Instance->ESR & CAN_ESR_EWGF; // Error Warning
  // 오류 발생 시 로그 출력
  if (error_code != HAL_CAN_ERROR_NONE || lec != 0 || fovr != 0 || boff || epvf || ewgf) {
      char error_msg[100];
      sprintf(error_msg, "Error: ID=0x%lX, Code=0x%08lX, LEC=0x%lX, REC=%ld, FOVR=%ld, BOFF=%ld, EPVF=%ld, EWGF=%ld, Time=%ld\r\n",
	      canRxHeader.StdId, error_code, lec, rec, fovr, boff, epvf, ewgf, HAL_GetTick());
      HAL_UART_Transmit(&huart3, (uint8_t *)error_msg, strlen(error_msg), 100);

      // 오류 유형별 처리
      switch (lec) {
	  case 0x3: // Stuff Error
	      sprintf(error_msg, "Stuff Error (ID=0x%lX): Check sender frame format\r\n", canRxHeader.StdId);
	      HAL_UART_Transmit(&huart3, (uint8_t *)error_msg, strlen(error_msg), 100);
	      break;
	  case 0x4: // CRC Error
	      sprintf(error_msg, "CRC Error (ID=0x%lX): Check sender data integrity\r\n", canRxHeader.StdId);
	      HAL_UART_Transmit(&huart3, (uint8_t *)error_msg, strlen(error_msg), 100);
	      break;
	  case 0x5: // Form Error
	      sprintf(error_msg, "Form Error (ID=0x%lX): Check sender frame structure\r\n", canRxHeader.StdId);
	      HAL_UART_Transmit(&huart3, (uint8_t *)error_msg, strlen(error_msg), 100);
	      break;
	  default:
	      break;
      }

      // FIFO 오버플로우 처리
      if (fovr) {
	  sprintf(error_msg, "FIFO Overrun (ID=0x%lX): Clearing FIFO and increasing polling frequency\r\n", canRxHeader.StdId);
	  HAL_UART_Transmit(&huart2, (uint8_t *)error_msg, strlen(error_msg), 100);
	  hcan1.Instance->RF0R |= CAN_RF0R_FOVR0; // FOVR 비트 클리어

	  // FIFO 비우기
	  while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0) {
	      HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHeader, RxData);
	  }
      }

      // Error Warning 상태
      if (ewgf)
      {
	dashboard_can_state_flag = 1;
      }

      // Error Passive 상태
      if (epvf)
      {
	dashboard_can_state_flag = 2;
      }

      // Bus-Off 상태
      if (boff) {
	  sprintf(error_msg, "Bus-Off: Reinitializing CAN, Time=%ld\r\n", HAL_GetTick());
	  HAL_UART_Transmit(&huart3, (uint8_t *)error_msg, strlen(error_msg), 100);
	  dashboard_can_state_flag = 0;
	  HAL_CAN_DeInit(&hcan1);
	  MX_CAN1_Init();
	  HAL_CAN_Start(&hcan1);
      }
  }

}
//void receive_data()
//{
//
//     if(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0)
//     {
//	  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHeader, &RxData[0]) == HAL_OK)
//	  {
//	    //printf("CAN Received. \r\n");
//	    printf("CAN RX: ID=0x%03lX DATA=%d %d %d %d %d %d %d %d\n",
//		    canRxHeader.StdId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);
//	    data_process(&RxData[0]);
//	  }
//     }
//}
float * get_rpm_ptr(void)
{
  return &car_rpm_tosend;
}
float * get_speed_ptr(void)
{
  return &car_speed_tosend;
}
uint8_t * get_can_state_flag_ptr()
{
  return &dashboard_can_state_flag;
}
uint8_t *get_back_warn_ptr()
{
  return &car_back_warn_tosend;
}
void data_process(uint8_t *rxbuffer)
{
   /*data processing*/
  steering_angle_tosend = ((int16_t)rxbuffer[0] << 8) | rxbuffer[1];
  car_rpm_tosend = ((uint16_t)rxbuffer[2] << 8) | rxbuffer[3];
  car_speed_tosend = ((uint16_t)rxbuffer[4] << 8) | rxbuffer[5];
  car_direction_tosend = (uint8_t)rxbuffer[6];
  car_back_warn_tosend = (uint8_t)rxbuffer[7];

  car_rpm_tosend = (float)(car_rpm_tosend / 100.0f);
  car_speed_tosend = (float)(car_speed_tosend / 1000.0f);
  printf("ANGLE : %d DIRECTION : %s BACK_WARN : %s \r\n", steering_angle_tosend, car_direction_tosend == 1? "FOWARD" : "BACKWARD", car_back_warn_tosend == 1 ? "TOO CLOSE" : "SAFE");
  printf("RPM : %.2f SPEED : %.2f \r\n", car_rpm_tosend, car_speed_tosend);

}


void esp8266_send_data(uint8_t * data)
{

}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHeader, &RxData[0]);
    if(canRxHeader.StdId == 0x000)
    {
       return ;
    }

    //printf("CAN RX: ID=0x%03lX DATA=%d %d %d %d %d %d %d %d\n",
    	      //canRxHeader.StdId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);
    data_process(&RxData[0]);
// if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &canRxHeader, &RxData[0]) == HAL_OK)
//    {
//      printf("CAN RX: ID=0x%03lX DATA=%d %d %d %d %d %d %d %d\n",
//	      canRxHeader.StdId, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);
//      data_process(&RxData[0]);
//    }
}


