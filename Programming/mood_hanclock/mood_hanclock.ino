/* mood_hanclock.ino
  Name:   Moodlight_HangeulClock
  Owner:  gogomaker
  Create: 2021-05-14, 5:26 PM

Project introduction_
  This .ino file is very important. Because it's me.

Project TODO

이 프로젝트는, 한글시계 프로그램과 매우 유사하게 제작 될 것이다.
다만 다른 것은 하나, 시간의 표기 방식(사실 날로먹기지 ㅋ)
그렇기에, 기존 한글시계 프로그램의 틀을 거의 가져와서 시간 표시하는 부분만 바꾸면 된다.

수정사안: 
이 시계는 버튼이 2개 들어간다. 그리고 각 버튼이 시간 및 분을 조정하는 데 사용이 된다.
버튼이 길게(1200ms) 눌렸을 때, 시간 또는 분이 증가한다.
또한, 짧은 시간에 대해 처리하는 if문을 날려야 한다.
*/
/* Arduino pin map
  3 - LED
  4 - LED(hour)
  5 - LED
  6 - LED
  9 - LED
 10 - LED
 11 - LED
 12 - SW_MIN
 13 - SW_HOUR
 A0 - brightness_potentiometers

 buttons will connection INPUT_PULLUP.
*/
/* LED pin map
  LED array
  PIN	  0	 1	2	3	4	5
  ARDU	11 10	9	6	5	3
*/

#include "src/functions.h"

//Charlie plexing mapping
byte pair[31][2] = // {LED(+) pin ,LED(-) pin}
{
  {LED_4,LED_5},  //PM
  {LED_5,LED_4},  //AM
  {LED_1,LED_HOUR},  //hour
  {LED_1,LED_0},  //MIN
  {LED_1,LED_5},  //H_1
  {LED_5,LED_1},  //H_2
  {LED_2,LED_3},  //H_3
  {LED_3,LED_2},  //H_4
  {LED_3,LED_4},  //H_5_F
  {LED_5,LED_3},  //H_6_F
  {LED_3,LED_5},  //H_7_F
  {LED_0,LED_1},  //h_9_F
  {LED_2,LED_4},  //H_5_L
  {LED_2,LED_5},  //H_7_L
  {LED_4,LED_2},  //H_8_L
  {LED_5,LED_2},  //H_9_L
  {LED_4,LED_3},  //H_TEN
  {LED_0,LED_2},  //M_1
  {LED_2,LED_0},  //M_2
  {LED_0,LED_3},  //M_3
  {LED_3,LED_0},  //M_4
  {LED_0,LED_4},  //M_5
  {LED_4,LED_0},  //M_6
  {LED_0,LED_5},  //M_7
  {LED_5,LED_0},  //M_8
  {LED_1,LED_2},  //M_9
  {LED_2,LED_1},  //M_10
  {LED_1,LED_3},  //M_20
  {LED_3,LED_1},  //M_30
  {LED_1,LED_4},  //M_40
  {LED_4,LED_1},  //M_50
};

//declaration variable
extern volatile unsigned long timer0_millis; //millis 오버플로우 대비 초기화
unsigned long time = 0;
byte sec, lastSec = 0;
byte hourPlus, minPlus = 0; //버튼이 눌릴 때 이곳에 값이 임시저장 됨
byte minRtc, hourRtc = 0; //RTC의 시간값이 저장됨
byte min, hour = 0;   //최종적으로 출력되는 시간값이 저장됨
bool isResetMillis = false;
byte bright = 0;  //255가 최대, 0이 최소, 가변저항 제어

byte tMSB, tLSB;  //RTC온도관련
float temp3231;

unsigned long bu_led_w = 0;
bool ledCheck = false;

bool bu_reading[2] = { HIGH, HIGH };  //버튼의 실제 상태를 표현하는 변수
bool bu_state[2] = { HIGH, HIGH };  //체터링을 거른 버튼의 상태를 표현하는 변수
bool last_bu_state[2] = { HIGH, HIGH }; //마지막 버튼 상태
unsigned long LastDebounceTime[2] = {0, 0};

unsigned long bu_t_w, last_bu_t_w = 0;  //시 버튼이 언제 눌렸는가
unsigned long bu_m_w, last_bu_m_w = 0;  //분 버튼이 언제 눌렸는가
unsigned long wait_t, wait_m = 0; //LED깜박일 때 사용
bool isblinkH, isblinkM = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Mood Light Hangeul Clock is turning ON");
  Serial.println("Clock start");
  //bright setting
  bright = map(analogRead(CONTROL_BRIGHT), 0, 1023, 0, 240);
  //button setting
  pinMode(BU_MIN, INPUT_PULLUP);
  pinMode(BU_HOUR, INPUT_PULLUP);
  //LED setting, other pins not define because they're analog pins.
  pinMode(LED_HOUR, OUTPUT);
}

void loop() {
  time = millis();
  bright = map(analogRead(CONTROL_BRIGHT), 0, 1023, 0, 240);
  get3231Date();
  if (!sec && !minRtc && !hourRtc) {	//millis 초기화
		if(!timer0_millis) isResetMillis = true;
		if (isResetMillis == true) {
			//Serial.println("Timer reset");
			bu_led_w = 0;
			LastDebounceTime[0] = 0;
			LastDebounceTime[1] = 0;
			bu_t_w = 0;
			last_bu_t_w = 0;
			wait_t = 0;
			wait_m = 0;
			time = 0;
			timer0_millis = 0;
			isResetMillis = false;
		}
	}
  //매 초마다
  if (sec != lastSec) {
    hour = (hourRtc + hourPlus) % 24;
    min = (minRtc + minPlus) % 60;
    if (!sec) {	//매 0초마다(1분 간격으로)
      if (hourPlus || minPlus) {  //RTC값 동기화
      //Serial.println("RTC set");
      set3231Date();
      }
      //Serial.println("updated");
    }
    lastSec = sec;
  }
  displayTime(hour, min); //시간출력, 계속 돌려야 하기에 loop에 넣었다
  void changeHour();
  void changeMin();
}