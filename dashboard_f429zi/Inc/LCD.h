/*
 * LCD.h
 *
 *  Created on: Jun 27, 2025
 *      Author: USER
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"
#include "com.h"
#include "ILI9341_Driver.h"
#include "ILI9341_GFX.h"
#include "fonts.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// 화면 해상도 정의
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// 게이지 위치 및 크기 정의 (240x320에 최적화)
#define GAUGE_RADIUS 60      // 반지름을 줄임
#define NEEDLE_LENGTH (GAUGE_RADIUS - 15)
#define RPM_CENTER_X 90      // 왼쪽 게이지 중심
#define RPM_CENTER_Y 120     // 상단 위치
#define SPEED_CENTER_X 230   // 오른쪽 게이지 중심
#define SPEED_CENTER_Y 120   // 상단 위치
#define ARROW_X  10
#define ARROW_Y  15
#define ARROW_SIZE    30
#define ARROW_WIDTH   30
#define ARROW_HEIGHT  30
#define ARROW_THICKNESS 3

#define M_PI            3.14159265358979323846
#define COLOR_WHITE     0xFFFF
#define COLOR_BLACK     0x0000
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_YELLOW    0xFFE0
#define COLOR_ORANGE    0xFD20
#define COLOR_GRAY      0x8410

//test function
void test(void);
//Draw line function
void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
//Draw RPM Frame
void DrawRPMFrame(void);
//Draw Speed Frame
void DrawSpeedFrame(void);
//Draw Warning Frame
void DrawWarningFrame(void);
//Draw Direction Frame
void DrawLeftArrow(void);
void DrawRightArrow(void);
//Initialize Dashboard(void)
void InitDashboard(void);
//converting rpm data to angle
float RPMToAngle(float rpm);
//Erase needle
void EraseNeedle(uint16_t center_x, uint16_t center_y, float angle);
//Draw needle
void DrawNeedle(uint16_t center_x, uint16_t center_y, float angle, uint16_t color);
//Updating RPM data
void UpdateRPM(float rpm);
//Updating Speed
void UpdateSpeed(float speed);
//Updating Warning Frames
void Updatewarningcode(uint8_t back_warn);
//Entire DashBoard Update function
void UpdateDashboard(float rpm, float speed);
#endif /* INC_LCD_H_ */
