/*
 * joystick.c
 *
 *  Created on: May 21, 2025
 *      Author: USER
 */

#include "joystick.h"
#include <stdio.h>

static uint16_t adcValue[2];
static uint8_t joystick_task_flag;

static uint8_t left;
static uint8_t cent;
static uint8_t right;
static uint8_t conversion_complete_flag = 0;
static uint8_t forward_flag = 0;

/*car steering*/
static uint16_t steering_angle_ccr;
static int16_t steering_angle;

static uint16_t car_direction = 0;

/*measuring motor_speed*/
static float car_rpm;
static float car_speed;
static uint8_t  edge_count;

void joystick_init(void)
{
   HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcValue, 2); // 2개 채널 데이터 전송

}

uint16_t * get_adcvalue_ptr(void)
{
  return adcValue;
}

uint8_t* get_joystick_task_flag_ptr()
{
  return &joystick_task_flag;
}

uint8_t* get_conversion_complete_flag_ptr()
{
  return &conversion_complete_flag;
}

void adc_task(uint32_t adc_channel)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = adc_channel;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_ADC_Start(&hadc1);
  if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
  {
     if(adc_channel == ADC_CHANNEL_1)
     {
       adcValue[0]=HAL_ADC_GetValue(&hadc1);
     }
     else if(adc_channel == ADC_CHANNEL_4)
     {
       adcValue[1]=HAL_ADC_GetValue(&hadc1);
     }
  }
  HAL_ADC_Stop(&hadc1);
}

void motor_forward(void)
{
  /*wheel direction*/
  HAL_GPIO_WritePin(IN1_GPIO_PORT, IN1_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(IN2_GPIO_PORT, IN2_PIN, GPIO_PIN_SET);

  /*encoder motor direction*/
  HAL_GPIO_WritePin(IN3_GPIO_PORT, IN3_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(IN4_GPIO_PORT, IN4_PIN, GPIO_PIN_RESET);
}
void motor_backward(void)
{
  /*wheel direction*/
   HAL_GPIO_WritePin(IN1_GPIO_PORT, IN1_PIN, GPIO_PIN_SET);
   HAL_GPIO_WritePin(IN2_GPIO_PORT, IN2_PIN, GPIO_PIN_RESET);

   /*encoder motor direction*/
   HAL_GPIO_WritePin(IN3_GPIO_PORT, IN3_PIN, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(IN4_GPIO_PORT, IN4_PIN, GPIO_PIN_SET);

}

void cal_carspeed()
{
  /*calculatring car speed*/
  /*
   * car_speed =
   * car_rpm =
   */

}
void joystick_task(void)
{
   /*Steering servo motor angle*/
  if(adcValue[0] <=1899)
  {
    steering_angle_ccr = 1167 + (uint16_t)((1500 - 1167) * (float)adcValue[0] / 1899);
    steering_angle = (int16_t)(0.1802 * steering_angle_ccr - 270.27);
  }
  else if(adcValue[0] > 2101)
  {
    steering_angle_ccr = 1500 + (uint16_t)((1833 - 1500) * ((float)(adcValue[0]- 2101) / (4000 - 2101)));
    steering_angle = (int16_t)(0.1802 * steering_angle_ccr - 270.27);
  }
  else
  {
    steering_angle_ccr = 1500;
  }
  steering_task(steering_angle_ccr);


  /*Applying real motor speed using adcValue[1]*/



  /*send datas through can*/
  sendMessage(&steering_angle, &car_rpm, &car_speed, &car_direction);


}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /*change forward, backward state*/
  if(GPIO_Pin == GPIO_PIN_13)
  {
    if(forward_flag == 1)
    {
      motor_backward();
      forward_flag = 0;
    }
    else if(forward_flag == 0)
    {
      motor_forward();
      forward_flag = 1;
    }
  }

  if(GPIO_Pin == GPIO_PIN_4)
  {
     uint8_t c2_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
     car_direction = (c2_state == 0) ? 1 : 0; // c2 low : forward high : backward
     edge_count++;
  }
  if(GPIO_Pin == GPIO_PIN_5)
  {
     uint8_t c1_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
     car_direction = (c1_state == 1) ? 1 : 0; // c1 high : forward low : backward
     edge_count++;
  }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM3)
  {
      car_rpm = (float)((60 * edge_count)/(0.01*PPR*RATIO*4));
      float omega = (2 * PI * car_rpm) / 60;
      car_speed = omega * 0.03;
      edge_count = 0;
  }
}

