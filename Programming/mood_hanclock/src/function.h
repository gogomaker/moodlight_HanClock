#ifndef __FUNCTION_H__
# define __FUNCTION_H__

#include <Arduino.h>

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

//Charlie plexing mapping
byte pair[31][2]
//declaration variable
extern volatile unsigned long timer0_millis; //millis 오버플로우 대비 초기화
unsigned long time;
byte sec, lastSec;
byte hourPlus, minPlus; //버튼이 눌릴 때 이곳에 값이 임시저장 됨
byte minRtc, hourRtc; //RTC의 시간값이 저장됨
byte min, hour;   //최종적으로 출력되는 시간값이 저장됨
bool isResetMillis;
byte bright;  //255가 최대, 0이 최소, 가변저항 제어

byte tMSB, tLSB;  //RTC온도관련
float temp3231;

unsigned long bu_led_w;
bool ledCheck;

bool bu_reading[2];  //버튼의 실제 상태를 표현하는 변수
bool bu_state[2];  //체터링을 거른 버튼의 상태를 표현하는 변수
bool last_bu_state[2]; //마지막 버튼 상태
unsigned long LastDebounceTime[2];

unsigned long bu_t_w, last_bu_t_w;  //시간 버튼이 언제 눌렸는가
unsigned long wait_t, wait_m; //LED깜박일 때 사용
bool timeCheck;
bool isblinkH, isblinkM;
byte tchange;
