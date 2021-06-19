#include "functions.h"
//LED 초기화 함수
void clearLED() {
	analogWrite(LED_0, 0);
	analogWrite(LED_1, 0);
	analogWrite(LED_2, 0);
	analogWrite(LED_3, 0);
	analogWrite(LED_4, 0);
	analogWrite(LED_5, 0);
}

int displayTime(int h, int m) {
	if(h < 12) {	// 오전 시 단위 처리 코드
		switch(h) {
		case 0:
			turnOnLED(LED_H_TEN);
			turnOnLED(LED_H_2);
			break;
		case 1:
			turnOnLED(LED_H_1);
			break;
		case 2:
			turnOnLED(LED_H_2);
			break;
		case 3:
			turnOnLED(LED_H_3);
			break;
		case 4:
			turnOnLED(LED_H_4);
			break;
		case 5:
			turnOnLED(LED_H_5_F);
			turnOnLED(LED_H_5_L);
			break;
		case 6:
			turnOnLED(LED_H_6_F);
			turnOnLED(LED_H_6_L);
			break;
		case 7:
			turnOnLED(LED_H_7_F);
			turnOnLED(LED_H_7_L);
			break;
		case 8:
			turnOnLED(LED_H_8_F);
			turnOnLED(LED_H_8_L);
			break;
		case 9:
			turnOnLED(LED_H_9_F);
			turnOnLED(LED_H_9_L);
			break;
		case 10:
			turnOnLED(LED_H_TEN);
			break;
		case 11:
			turnOnLED(LED_H_TEN);
			turnOnLED(LED_H_1);
			break;
		
		default:
			Serial.println("AM_case_error");
			break;
		}
		turnOnLED(LED_AM);
		delay(4); //delay를 millis를 활용하는 코드로 변경 or delayMicro~~
	}
	else if (h > 11) {	//오후 시 단위 처리 코드
		switch (h) {
		case 12:
			turnOnLED(LED_H_TEN);
			turnOnLED(LED_H_2);
			break;
		case 13:
			turnOnLED(LED_H_1);
			break;
		case 14:
			turnOnLED(LED_H_2);
			break;
		case 15:
			turnOnLED(LED_H_3);
			break;
		case 16:
			turnOnLED(LED_H_4);
			break;
		case 17:
			turnOnLED(LED_H_5_F);
			turnOnLED(LED_H_5_L);
			break;
		case 18:
			turnOnLED(LED_H_6_F);
			turnOnLED(LED_H_6_L);
			break;
		case 19:
			turnOnLED(LED_H_7_F);
			turnOnLED(LED_H_7_L);
			break;
		case 20:
			turnOnLED(LED_H_8_F);
			turnOnLED(LED_H_8_L);
			break;
		case 21:
			turnOnLED(LED_H_9_F);
			turnOnLED(LED_H_9_L);
			break;
		case 22:
			turnOnLED(LED_H_TEN);
			break;
		case 23:
			turnOnLED(LED_H_TEN);
			turnOnLED(LED_H_1);
			break;
		
		default:
			Serial.println("PM_case_error");
			break;
		}
		turnOnLED(LED_PM);
		delay(4); //delay를 millis를 활용하는 코드로 변경 or delayMicro~~
	}
	//분 단위 처리 코드
	if(m) turnOnLED(LED_MIN);
	int ten = m / 10;
	switch (ten) {	//분 십의 단위 처리 코드
	case 1:
		turnOnLED(LED_M_10); 
		turnOnLED(LED_MIN);
		break;
	case 2:
		turnOnLED(LED_M_20);
		turnOnLED(LED_M_10);
		break;
	case 3:
		turnOnLED(LED_M_30);
		turnOnLED(LED_M_10);
		break;
	case 4:
		turnOnLED(LED_M_40);
		turnOnLED(LED_M_10);
		break;
	case 5:
		turnOnLED(LED_M_50);
		turnOnLED(LED_M_10);
		break;
	
	default:
		Serial.println("minTen_case_error");
		break;
	}
	m -= ten * 10;
	switch (m) {	//분 일의 단위 처리 코드
		case 1: turnOnLED(LED_M_1); break;
		case 2: turnOnLED(LED_M_2); break;
		case 3: turnOnLED(LED_M_3); break;
		case 4: turnOnLED(LED_M_4); break;
		case 5: turnOnLED(LED_M_5); break;
		case 6: turnOnLED(LED_M_6); break;
		case 7: turnOnLED(LED_M_7); break;
		case 8: turnOnLED(LED_M_8); break;
		case 9: turnOnLED(LED_M_9); break;
		default:
			Serial.println("min_case_error");
			break;
	}
	
}

//LED 켜는 함수, 인수값으로 받은 LED의 번호를 켠다.
int turnOnLED(int n){
	clearLED();
	analogWrite(pair[n][0], bright);
	analogWrite(pair[n][1], 0);
}

//시간 버튼의 상태를 체크하는 함수
void changeHour() {
	int reading = digitalRead(BU_HOUR);
	if (reading != last_bu_state[0]) {
		LastDebounceTime[0] = time;
	}
	if ((time - LastDebounceTime[0]) > debounceDelay) {
		if (reading != bu_state[0]) {
			bu_state[0] = reading;
			if (bu_state[0] == LOW) {	//버튼이 눌렸다면
				bu_t_w = time;
			}
			else {	//버튼이 때졌다면
				if (time < bu_t_w + bu_interval) {	//버튼 길게 눌렀으면
					//Serial.println("hour plus");
					hourPlus++;
					if (hourPlus > 23) hourPlus = 0;
					hour = (hourRtc + hourPlus) % 24;
				}
			}
		}
	}
	last_bu_state[0] = reading;
}

//분 버튼의 상태를 체크하는 함수
void changeMin() {
	int reading = digitalRead(BU_MIN);
	if (reading != last_bu_state[1]) {
		LastDebounceTime[1] = time;
	}
	if ((time - LastDebounceTime[1]) > debounceDelay) {
		if (reading != bu_state[1]) {
			bu_state[1] = reading;
			if (bu_state[1] == LOW) {	//버튼이 눌렸다면
				bu_m_w = time;
			}
			else {	//버튼이 때졌다면
				if (time < bu_m_w + bu_interval) {	//버튼 길게 눌렀으면
						//Serial.println("min plus");
						minPlus++;
						if (minPlus > 59) minPlus = 0;
						min = (minRtc + minPlus) % 60;
				}
			}
		}
	}
	last_bu_state[1] = reading;
}

//RTC처리함수
byte decToBcd(byte val) {
	return ((val / 10 * 16) + (val % 10));
}

void set3231Date() {
	//year = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//month = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//date = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//hour = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//min = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//sec = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//day = (byte)(Serial.read() - 48);
	if (!min) hour++;
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0x00);
	Wire.write(sec);
	Wire.write(decToBcd(min));
	Wire.write(decToBcd(hour));
	//Wire.write(decToBcd(day));
	//Wire.write(decToBcd(date));
	//Wire.write(decToBcd(month));
	//Wire.write(decToBcd(year));
	Wire.endTransmission();

	minPlus = hourPlus = 0;
}

void get3231Date() {
	// send request to receive data starting at register 0
	Wire.beginTransmission(DS3231_I2C_ADDRESS); // 104 is DS3231 device address
	Wire.write(0x00); // start at register 0
	Wire.endTransmission();
	Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // request seven bytes

	if (Wire.available()) {
		sec = Wire.read(); // get seconds
		minRtc = Wire.read(); // get minutes
		hourRtc = Wire.read();   // get hours
	//	day = Wire.read();
	//	date = Wire.read();
	//	month = Wire.read(); //temp month
	//	year = Wire.read();

		sec = (((sec & B11110000) >> 4) * 10 + (sec & B00001111)); // convert BCD to decimal
		minRtc = (((minRtc & B11110000) >> 4) * 10 + (minRtc & B00001111)); // convert BCD to decimal
		hourRtc = (((hourRtc & B00110000) >> 4) * 10 + (hourRtc & B00001111)); // convert BCD to decimal (assume 24 hour mode)
	//	day = (day & B00000111); // 1-7
	//	date = (((date & B00110000) >> 4) * 10 + (date & B00001111)); // 1-31
	//	month = (((month & B00010000) >> 4) * 10 + (month & B00001111)); //msb7 is century overflow
	//	year = (((year & B11110000) >> 4) * 10 + (year & B00001111));
	}
	else {
		//oh noes, no data!
	}

	/*switch (day) {
	case 1:
		strcpy(weekDay, "Sun");
		break;
	case 2:
		strcpy(weekDay, "Mon");
		break;
	case 3:
		strcpy(weekDay, "Tue");
		break;
	case 4:
		strcpy(weekDay, "Wed");
		break;
	case 5:
		strcpy(weekDay, "Thu");
		break;
	case 6:
		strcpy(weekDay, "Fri");
		break;
	case 7:
		strcpy(weekDay, "Sat");
		break;
	}*/
}

float get3231Temp() {
	//temp registers (11h-12h) get updated automatically every 64s
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0x11);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_I2C_ADDRESS, 2);

	if (Wire.available()) {
		tMSB = Wire.read(); //2's complement int portion
		tLSB = Wire.read(); //fraction portion

		temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
		temp3231 += ((tLSB >> 6) * 0.25); //only care about bits 7 & 8
	}
	else {
		//error! no data!
	}
	return temp3231;
}

void showSerialTime() {

	Serial.print(hour, DEC);
	Serial.print(":");
	Serial.print(min, DEC);
	Serial.print(":");
	Serial.print(sec, DEC);
	Serial.print(" - Temp: ");
	Serial.println(get3231Temp());

	Serial.print(hourRtc);
	Serial.print(", ");
	Serial.print(hourPlus);
	Serial.print(" ; ");
	Serial.print(minRtc);
	Serial.print(",");
	Serial.println(minPlus);
	Serial.println();
}