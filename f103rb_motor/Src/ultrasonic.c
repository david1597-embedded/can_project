/*
 * ultrasonic.c
 *
 *  Created on: May 21, 2025
 *      Author: USER
 */


#include "ultrasonic.h"
#include "joystick.h"
static uint16_t IC_Value_1 = 0;
static uint16_t IC_Value_2 = 0;
static uint16_t echo_time = 0;

static ULTRASONIC ultra1;
void ultra_module_Init()
{
  ultra_Init(&ultra1, GPIOB, GPIO_PIN_0);
}

void ultra_Init(ULTRASONIC *ultra, GPIO_TypeDef *port, uint16_t pinNumber)
{
  ultra->port = port;
  ultra->pinNumber = pinNumber;
  ultra->capture_flag = 0;
  ultra->distance = 0;
}

void ultra_delay(uint16_t us)
{

  __HAL_TIM_SET_COUNTER(&htim4,0);
   while((__HAL_TIM_GET_COUNTER(&htim4))<us);

}
void ultra_Trigger(ULTRASONIC *ultra, uint32_t TIM_IT_CCx )
{
    HAL_GPIO_WritePin(ultra->port, ultra->pinNumber, GPIO_PIN_RESET);
    ultra_delay(1);

    HAL_GPIO_WritePin(ultra->port, ultra->pinNumber, GPIO_PIN_SET);
    ultra_delay(10);
    HAL_GPIO_WritePin(ultra->port, ultra->pinNumber, GPIO_PIN_RESET);

    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CCx);
}

ULTRASONIC * get_ultra_ptr()
{
    //printf("ang gimozzi \r\n");
    return &ultra1;
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
   if(htim->Instance == TIM4)
   {
     if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
     {
        if(ultra1.capture_flag == 0)
        {
          IC_Value_1 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);
	  ultra1.capture_flag=1;
	  __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else if(ultra1.capture_flag == 1)
        {
	  IC_Value_2 = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1);
	  if(IC_Value_2 > IC_Value_1)
	  {
	    echo_time=IC_Value_2 - IC_Value_1;
	  }
	  else
	  {
	    echo_time=(0xFFFF - IC_Value_1) + IC_Value_2;
	  }
	  ultra1.distance=echo_time/58;
          if(ultra1.distance < 20)
          {
             *get_back_warn_ptr() = 1;
          }
          else
          {
            *get_back_warn_ptr() = 0;
          }
	  ultra1.capture_flag = 0;
	  __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
	  __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC1);
        }
     }
   }

}

