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
extern unsigned long bu_led_w;
extern bool ledCheck;
extern bool bu_reading[2];  //버튼의 실제 상태를 표현하는 변수
extern bool bu_state[2];  //체터링을 거른 버튼의 상태를 표현하는 변수
extern bool last_bu_state[2]; //마지막 버튼 상태
extern unsigned long LastDebounceTime[2];
extern unsigned long bu_t_w, last_bu_t_w;  //시간 버튼이 언제 눌렸는가
extern unsigned long wait_t, wait_m; //LED깜박일 때 사용
extern bool timeCheck;
extern bool isblinkH, isblinkM;
extern byte tchange;

//LED처리 함수
int turnLED();

//버튼처리함수
void changeTimeButton();
void longTimeButton();
byte decToBcd(byte val);
//RTC처리함수
void set3231Date();
void get3231Date();
float get3231Temp();
void showSerialTime();

#endif