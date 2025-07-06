/*
 * LCD.c
 *
 *  Created on: Jun 27, 2025
 *      Author: USER
 */

#include "LCD.h"

// 바늘 각도 저장용
static float rpm_last_angle = 225.0;
static float speed_last_angle = 225.0;
// 경계 체크 함수


// ========== 왼쪽을 향하는 화살표 (수정된 버전) ==========
void DrawLeftArrow() {
    // 화살표 꼭지점 계산
    uint16_t arrowTipX = ARROW_X;
    uint16_t arrowTipY = ARROW_Y + ARROW_HEIGHT/2;
    uint16_t arrowBaseX = ARROW_X + ARROW_WIDTH/2;

    // 몸통 높이를 전체 높이의 1/3로 설정
    uint16_t bodyHeight = ARROW_HEIGHT / 3;
    uint16_t bodyTopY = ARROW_Y + (ARROW_HEIGHT - bodyHeight) / 2;
    uint16_t bodyBottomY = bodyTopY + bodyHeight;

    // 화살표 상단 라인 (외곽)
    ILI9341_DrawLine(arrowTipX, arrowTipY, arrowBaseX, ARROW_Y, COLOR_WHITE);

    // 화살표 하단 라인 (외곽)
    ILI9341_DrawLine(arrowTipX, arrowTipY, arrowBaseX, ARROW_Y + ARROW_HEIGHT, COLOR_WHITE);

    // 화살촉에서 몸통으로 연결하는 라인들
    ILI9341_DrawLine(arrowBaseX, ARROW_Y, arrowBaseX, bodyTopY, COLOR_WHITE);
    ILI9341_DrawLine(arrowBaseX, ARROW_Y + ARROW_HEIGHT, arrowBaseX, bodyBottomY, COLOR_WHITE);

    // 화살표 몸통 상단 라인
    ILI9341_DrawLine(arrowBaseX, bodyTopY, ARROW_X + ARROW_WIDTH, bodyTopY, COLOR_WHITE);

    // 화살표 몸통 하단 라인
    ILI9341_DrawLine(arrowBaseX, bodyBottomY, ARROW_X + ARROW_WIDTH, bodyBottomY, COLOR_WHITE);

    // 화살표 몸통 우측 라인
    ILI9341_DrawLine(ARROW_X + ARROW_WIDTH, bodyTopY, ARROW_X + ARROW_WIDTH, bodyBottomY, COLOR_WHITE);

}

// ========== 오른쪽을 향하는 화살표 ==========
void DrawRightArrow() {
    // 대시보드 좌우 대칭을 위한 X 좌표 계산 (320 - 왼쪽화살표X)
    uint16_t rightArrowX = 320 - ARROW_X - ARROW_WIDTH;

    // 화살표 꼭지점 계산 (오른쪽 끝)
    uint16_t arrowTipX = rightArrowX + ARROW_WIDTH;
    uint16_t arrowTipY = ARROW_Y + ARROW_HEIGHT/2;
    uint16_t arrowBaseX = rightArrowX + ARROW_WIDTH/2;

    // 몸통 높이를 전체 높이의 1/3로 설정
    uint16_t bodyHeight = ARROW_HEIGHT / 3;
    uint16_t bodyTopY = ARROW_Y + (ARROW_HEIGHT - bodyHeight) / 2;
    uint16_t bodyBottomY = bodyTopY + bodyHeight;

    // 화살표 상단 라인 (외곽)
    ILI9341_DrawLine(arrowTipX, arrowTipY, arrowBaseX, ARROW_Y, COLOR_WHITE);

    // 화살표 하단 라인 (외곽)
    ILI9341_DrawLine(arrowTipX, arrowTipY, arrowBaseX, ARROW_Y + ARROW_HEIGHT, COLOR_WHITE);

    // 화살촉에서 몸통으로 연결하는 라인들
    ILI9341_DrawLine(arrowBaseX, ARROW_Y, arrowBaseX, bodyTopY, COLOR_WHITE);
    ILI9341_DrawLine(arrowBaseX, ARROW_Y + ARROW_HEIGHT, arrowBaseX, bodyBottomY, COLOR_WHITE);

    // 화살표 몸통 상단 라인
    ILI9341_DrawLine(rightArrowX, bodyTopY, arrowBaseX, bodyTopY, COLOR_WHITE);

    // 화살표 몸통 하단 라인
    ILI9341_DrawLine(rightArrowX, bodyBottomY, arrowBaseX, bodyBottomY, COLOR_WHITE);

    // 화살표 몸통 좌측 라인
    ILI9341_DrawLine(rightArrowX, bodyTopY, rightArrowX, bodyBottomY, COLOR_WHITE);

}
// RPM 프레임 그리기 (240x320에 최적화)
void DrawRPMFrame(void)
{
     ILI9341_DrawHollowCircle(RPM_CENTER_X, RPM_CENTER_Y, GAUGE_RADIUS, COLOR_WHITE);
     ILI9341_DrawHollowCircle(RPM_CENTER_X, RPM_CENTER_Y, GAUGE_RADIUS-1, COLOR_WHITE);

     //Central Dot
     ILI9341_DrawFilledCircle(RPM_CENTER_X, RPM_CENTER_Y, 4, COLOR_WHITE);

     // 눈금 그리기
     for (int i = 0; i <= 30; i++) {
         float angle = 90 + (i * 9.0);
         float angle_rad = angle * M_PI / 180.0;
         uint16_t x1 = RPM_CENTER_X + GAUGE_RADIUS * cos(angle_rad);
         uint16_t y1 = RPM_CENTER_Y + GAUGE_RADIUS * sin(angle_rad);

         uint16_t x2 = RPM_CENTER_X + (GAUGE_RADIUS-8) * cos(angle_rad);
         uint16_t y2 = RPM_CENTER_Y + (GAUGE_RADIUS-8) * sin(angle_rad);

         // 디버깅 출력 (시리얼 모니터 지원 시 활성화)
         printf("RPM Tick %d: x1=%d, y1=%d, x2=%d, y2=%d\n", i, x1, y1, x2, y2);

         ILI9341_DrawLine(x1, y1, x2, y2, COLOR_WHITE);
     }

     // RPM 숫자 표시 (위치 조정)
     ILI9341_DrawText("0", FONT1, RPM_CENTER_X -2, RPM_CENTER_Y + 55, COLOR_WHITE, COLOR_BLACK);//0
     ILI9341_DrawText("1", FONT1, RPM_CENTER_X - 45, RPM_CENTER_Y +42, COLOR_WHITE, COLOR_BLACK);//0
     ILI9341_DrawText("2", FONT1, RPM_CENTER_X - 65, RPM_CENTER_Y , COLOR_WHITE, COLOR_BLACK);//0
     ILI9341_DrawText("3", FONT1, RPM_CENTER_X - 45, RPM_CENTER_Y -48, COLOR_WHITE, COLOR_BLACK);//0
     ILI9341_DrawText("4", FONT1, RPM_CENTER_X -2 , RPM_CENTER_Y -65, COLOR_WHITE, COLOR_BLACK);//0
     ILI9341_DrawText("5", FONT1, RPM_CENTER_X +45, RPM_CENTER_Y -46, COLOR_YELLOW, COLOR_BLACK);//0
     ILI9341_DrawText("6", FONT1, RPM_CENTER_X + 58, RPM_CENTER_Y -2, COLOR_RED, COLOR_BLACK);//0


     // 라벨
     ILI9341_DrawText("RPM", FONT2, RPM_CENTER_X+14, RPM_CENTER_Y+15, COLOR_WHITE, COLOR_BLACK);
     ILI9341_DrawText("x100", FONT1, RPM_CENTER_X+18, RPM_CENTER_Y+3, COLOR_WHITE, COLOR_BLACK);
}

// 속도 프레임 그리기 (240x320에 최적화)
void DrawSpeedFrame(void)
{
    ILI9341_DrawHollowCircle(SPEED_CENTER_X, SPEED_CENTER_Y, GAUGE_RADIUS, COLOR_WHITE);
    ILI9341_DrawHollowCircle(SPEED_CENTER_X, SPEED_CENTER_Y, GAUGE_RADIUS-1, COLOR_WHITE);
    ILI9341_DrawFilledCircle(SPEED_CENTER_X, SPEED_CENTER_Y, 4, COLOR_WHITE);

    // 눈금 그리기
    for (int i = 0; i <= 30 ; i++) {
        float angle = 90 + (i * 9.0);
        float angle_rad = angle * M_PI / 180.0;
        uint16_t x1 = SPEED_CENTER_X + GAUGE_RADIUS * cos(angle_rad);
        uint16_t y1 = SPEED_CENTER_Y + GAUGE_RADIUS * sin(angle_rad);
        uint16_t x2 = SPEED_CENTER_X + (GAUGE_RADIUS-8) * cos(angle_rad);
        uint16_t y2 = SPEED_CENTER_Y + (GAUGE_RADIUS-8) * sin(angle_rad);
        ILI9341_DrawLine(x1, y1, x2, y2, COLOR_WHITE);
    }

    // 속도 숫자 표시 (위치 조정)
    ILI9341_DrawText("0", FONT1, SPEED_CENTER_X-2, SPEED_CENTER_Y + 55, COLOR_WHITE, COLOR_BLACK);
    ILI9341_DrawText("20", FONT1, SPEED_CENTER_X - 51, SPEED_CENTER_Y +40, COLOR_WHITE, COLOR_BLACK);//0
    ILI9341_DrawText("40", FONT1, SPEED_CENTER_X - 64, SPEED_CENTER_Y, COLOR_WHITE, COLOR_BLACK);//0
    ILI9341_DrawText("60", FONT1, SPEED_CENTER_X - 52, SPEED_CENTER_Y - 46, COLOR_WHITE, COLOR_BLACK);//0
    ILI9341_DrawText("80", FONT1, SPEED_CENTER_X - 2, SPEED_CENTER_Y-68, COLOR_WHITE, COLOR_BLACK);//0
    ILI9341_DrawText("100", FONT1, SPEED_CENTER_X + 44, SPEED_CENTER_Y - 46, COLOR_YELLOW, COLOR_BLACK);//0
    ILI9341_DrawText("100", FONT1, SPEED_CENTER_X + 56, SPEED_CENTER_Y , COLOR_RED, COLOR_BLACK);//0

    // 라벨
    ILI9341_DrawText("cm/s", FONT2, SPEED_CENTER_X+14, SPEED_CENTER_Y+18, COLOR_WHITE, COLOR_BLACK);
}
void DrawWarningFrame()
{
    ILI9341_DrawTriangle(10, 192, 26,160, 42,192,COLOR_WHITE);
}
// 대시보드 초기화 (240x320 해상도)
void InitDashboard(void)
{
    ILI9341_DrawFilledRectangleCoord(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK);

    // 제목 표시 (상단에 위치)
    ILI9341_DrawText("CAR DASH BOARD ", FONT4, 75, 10, COLOR_WHITE, COLOR_BLACK);

    // 두 게이지 모두 그리기
    DrawRPMFrame();
    DrawSpeedFrame();
    DrawWarningFrame();
    DrawLeftArrow();
    DrawRightArrow();

    // 초기값으로 바늘 설정
    UpdateRPM(0);
    UpdateSpeed(0);
    //Updatewarningcode(0);

    // 하단에 추가 정보 표시 (선택사항)
    ILI9341_DrawText("Motor Control Dashboard", FONT1, 40, 280, COLOR_WHITE, COLOR_BLACK);
}

// RPM 각도 계산
float RPMToAngle(float rpm)
{
    if (rpm < 0) rpm = 0;
    if (rpm > 300) rpm = 300;
    return 90.0 + (rpm * 270.0 / 600.0);
}

// 속도 각도 계산
float SpeedToAngle(float speed)
{
    if (speed < 0) speed = 0;
    if (speed > 120) speed = 120;
    return 90.0 + (speed * 270.0 / 120.0);
}

// 바늘 지우기
void EraseNeedle(uint16_t center_x, uint16_t center_y, float angle)
{
    float angle_rad = angle * M_PI / 180.0;
    uint16_t end_x = center_x + NEEDLE_LENGTH * cos(angle_rad);
    uint16_t end_y = center_y + NEEDLE_LENGTH * sin(angle_rad);
    ILI9341_DrawLine(center_x, center_y, end_x, end_y, COLOR_BLACK);
}

// 바늘 그리기
void DrawNeedle(uint16_t center_x, uint16_t center_y, float angle, uint16_t color)
{
    float angle_rad = angle * M_PI / 180.0;
    uint16_t end_x = center_x + NEEDLE_LENGTH * cos(angle_rad);
    uint16_t end_y = center_y + NEEDLE_LENGTH * sin(angle_rad);
    ILI9341_DrawLine (center_x, center_y, end_x, end_y, color);
    ILI9341_DrawFilledCircle(center_x, center_y, 4, COLOR_WHITE);
}

// RPM 업데이트
void UpdateRPM(float rpm)
{
 // printf("111111 \r\n");
    uint16_t needle_color;
    if (rpm >= 500) needle_color = COLOR_RED;
    else if (rpm >= 400) needle_color = COLOR_ORANGE;
    else if (rpm >= 300) needle_color = COLOR_YELLOW;
    else needle_color = COLOR_GREEN;
  //  printf("22222 \r\n");
    EraseNeedle(RPM_CENTER_X, RPM_CENTER_Y, rpm_last_angle);
    float new_angle = RPMToAngle(rpm);
    DrawNeedle(RPM_CENTER_X, RPM_CENTER_Y, new_angle, needle_color);
    rpm_last_angle = new_angle;

    // 숫자 표시 영역 조정
    char rpm_str[6];
    sprintf(rpm_str, "%4.0f", rpm);
   // printf("3333 \r\n");
    //ILI9341_DrawFilledRectangleCoord(RPM_CENTER_X-20, RPM_CENTER_Y+5, RPM_CENTER_X+20, RPM_CENTER_Y+20, COLOR_BLACK);
    //ILI9341_DrawText(rpm_str, FONT2, RPM_CENTER_X-15, RPM_CENTER_Y+8, COLOR_WHITE, COLOR_BLACK);
}

// 속도 업데이트
void UpdateSpeed(float speed)
{
    uint16_t needle_color;
    if (speed >= 100) needle_color = COLOR_RED;
    else if (speed >= 80) needle_color = COLOR_ORANGE;
    else if (speed >= 60) needle_color = COLOR_YELLOW;
    else needle_color = COLOR_GREEN;

    EraseNeedle(SPEED_CENTER_X, SPEED_CENTER_Y, speed_last_angle);
    float new_angle = SpeedToAngle(speed);
    DrawNeedle(SPEED_CENTER_X, SPEED_CENTER_Y, new_angle, needle_color);
    speed_last_angle = new_angle;

    // 숫자 표시 영역 조정
    char speed_str[4];
    sprintf(speed_str, "%3.0f", speed);
    //ILI9341_DrawFilledRectangleCoord(SPEED_CENTER_X-15, SPEED_CENTER_Y+5, SPEED_CENTER_X+15, SPEED_CENTER_Y+20, COLOR_BLACK);
    ILI9341_DrawText(speed_str, FONT2, SPEED_CENTER_X + 16, SPEED_CENTER_Y+8, COLOR_WHITE, COLOR_BLACK);
}

void Updatewarningcode(uint8_t back_warn)
{
    static uint8_t blink_toggle_flag = 0;


    if(back_warn == 0)
    {
      printf("keep dar \r\n");
      ILI9341_DrawFilledTriangle(10, 192, 26,160, 42,192,COLOR_BLACK);
    }
    else if(back_warn == 1)
    {
      if(blink_toggle_flag == 0)
      {
	ILI9341_DrawFilledTriangle(10, 192, 26,160, 42,192,COLOR_RED);
	blink_toggle_flag = 1 ;
      }
      else if(blink_toggle_flag == 1)
      {
	printf("epqreqwrqwt \r\n");
	ILI9341_DrawFilledTriangle(10, 192, 26,160, 42,192,COLOR_BLACK);
	blink_toggle_flag = 0 ;
      }
    }

}
// 대시보드 업데이트
void UpdateDashboard(float rpm, float speed)
{
  //printf("update dashboard entered \r\n");
    UpdateRPM(rpm);
    UpdateSpeed(speed);
    //Updatewarningcode(0);
}

void dashboard_process()
{
    //실제 RPM 과 실제 선속도를 받아와서 적용
   //UpdateDashboard(실제 rpm, 실제 선속도
}


