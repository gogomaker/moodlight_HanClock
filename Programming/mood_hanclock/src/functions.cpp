#include "functions.h"

//LED 켜는 함수, 인수값으로 받은 LED의 번호를 켠다.
int turnLED(){
	
}

//버튼처리함수
void changeTimeButton() {
	int reading = digitalRead(2);
	if (reading != last_bu_state[0]) {
		LastDebounceTime[0] = time;
	}
	if ((time - LastDebounceTime[0]) > debounceDelay) {
		if (reading != bu_state[0]) {
			bu_state[0] = reading;
			if (bu_state[0] == LOW) {
				//Serial.println("Time_pressed");
				bu_t_w = time;
				timeCheck = true;
			}
			else {
				if (time < bu_t_w + bu_interval) {
					if (tchange == 1) {
						//Serial.println("hour plus");
						hourPlus++;
						if (hourPlus > 23) hourPlus = 0;
						hour = (hourRtc + hourPlus) % 24;
					}
					else if(tchange == 2) {
						//Serial.println("min plus");
						minPlus++;
						if (minPlus > 59) minPlus = 0;
						min = (minRtc + minPlus) % 60;
					}
					timeCheck = false;
				}
			}
		}
	}
	last_bu_state[0] = reading;
}

void longTimeButton() {
	if (timeCheck == true) {
		if ((time - bu_t_w) >= bu_interval) {
			tchange += 1;
			if (tchange > 2) tchange = 0;
			if (tchange == 1) {
				//Serial.println("hour off");
				if ((hour == 0 || hour == 12) && min == 0) {
					//정각이라면
				}
				else {//정각이 아니라면
				}
			}
			else if (tchange == 2) {
				//Serial.println("min off");
				if (min == 0) {
				}
				else {
				}
			}
			timeCheck = false;
			//Serial.print("mode change: ");
			//Serial.println(tchange);
		}
	}
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