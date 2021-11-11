#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

const byte buttonPins[5] = { A0, A1, A2, A3, A4 };
const byte ledPins[5] = { 8, 9, 10, 11, 12 };

byte buttons = 0b11111;
byte leds[5]; // 0 partout par défaut

enum {
	INIT,
	WAIT_START,
	WAIT_FIRST
} step;

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	for(int i = 0 ; i <= 4; i++) {
		pinMode(buttonPins[i], INPUT_PULLUP);
		pinMode(ledPins[i], OUTPUT);
	}

	Serial.println("Init");

	byte l = 1;
	for (int i = 0; i < 10; i++) {
		for(int i = 0 ; i <= 4; i++) {
			digitalWrite(ledPins[i], l);
		}
		delay(100);
		l = !l;
	}

	step = WAIT_START;

	Serial.println("Ready.");
}

void loop() {
	if (step == WAIT_START) {

		if (digitalRead(buttonPins[0]) == 0) {
			Serial.println("Waiting first push");

			for(byte i = 1 ; i <= 4; i++) {
				digitalWrite(ledPins[i], 0);
			}

			digitalWrite(ledPins[0], 1);
			step = WAIT_FIRST;
		}

	} else if (step == WAIT_FIRST) {

		byte b = PINC & 0b11110;
		if (b != 0b11110) {
			for(byte i = 1, m = 0b00010 ; i <= 4; i++) {
				if ((b & m) == 0) {
					Serial.print("Winner "); Serial.println(i);
					digitalWrite(ledPins[i], 1);
				}
				m <<= 1;
			}
			digitalWrite(ledPins[0], 0);
			step = WAIT_START;
		}

	}
}
