#ifndef __FUNCTIONS_H__
# define __FUNCTIONS_H__

#include <Arduino.h>
#include <Wire.h>

//define pin map
#define LED_0 11
#define LED_1 10
#define LED_2 9
#define LED_3 6
#define LED_4 5
#define LED_5 3
#define LED_HOUR 4
#define BU_MIN 12
#define BU_HOUR 13
#define CONTROL_BRIGHT A0
//define LED numbers
#define LED_PM 0
#define LED_AM 1
#define LED_HOUR 2
#define LED_MIN 3
#define LED_H_1 4
#define LED_H_2 5
#define LED_H_3 6
#define LED_H_4 7
#define LED_H_5_F 8
#define LED_H_6_F 9
#define LED_H_7_F 10
#define LED_H_8_F 9
#define LED_H_9_F 11
#define LED_H_5_L 12
#define LED_H_6_L 12
#define LED_H_7_L 13
#define LED_H_8_L 14
#define LED_H_9_L 15
#define LED_H_TEN 16
#define LED_M_1 17
#define LED_M_2 18
#define LED_M_3 19
#define LED_M_4 20
#define LED_M_5 21
#define LED_M_6 22
#define LED_M_7 23
#define LED_M_8 24
#define LED_M_9 25
#define LED_M_10 26
#define LED_M_20 27
#define LED_M_30 28
#define LED_M_40 29
#define LED_M_50 30
//define other...
#define DS3231_I2C_ADDRESS 104
#define FLICKER_TIME 5  //LED flicker ms
#define debounceDelay 50  //button debounce removal delay
#define bu_interval 1200

extern byte pair[31][2];
extern unsigned long time;
extern byte sec, lastSec;
extern byte hourPlus, minPlus; //버튼이 눌릴 때 이곳에 값이 임시저장 됨
extern byte minRtc, hourRtc; //RTC의 시간값이 저장됨
extern byte min, hour;   //최종적으로 출력되는 시간값이 저장됨
extern bool isResetMillis;
extern byte bright;  //255가 최대, 0이 최소, 가변저항 제어
extern byte tMSB, tLSB;  //RTC온도관련
extern float temp3231;
extern bool bu_reading[2];  //버튼의 실제 상태를 표현하는 변수
extern bool bu_state[2];  //체터링을 거른 버튼의 상태를 표현하는 변수
extern bool last_bu_state[2]; //마지막 버튼 상태
extern unsigned long LastDebounceTime[2];
extern unsigned long bu_t_w, last_bu_t_w;  //시 버튼이 언제 눌렸는가
extern unsigned long bu_m_w, last_bu_m_w;  //분 버튼이 언제 눌렸는가

void clearLED();
int turnOnLED(int n);
int displayTime(int h, int m);
void changeHour();
void changeMin();
byte decToBcd(byte val);
void set3231Date();
void get3231Date();
float get3231Temp();
void showSerialTime();

#endif