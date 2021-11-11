#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define SPEAKER 7
#define LONG_PRESS_DELAY 1000

// Si b , Ré b , Fa , Si b
const int notes[4] = { 466, 554, 697, 932 };

const byte buttonPins[5] = { A0, A1, A2, A3, A4 };
const byte ledPins[5] = { 8, 9, 10, 11, 12 };

byte buttons = 0b11111;
byte leds[5]; // 0 partout par défaut

enum {
	QUIZZ_WAIT_START,
	QUIZZ_WAIT_FIRST,
	SIMON_TODO
} step;

void quizzSound() {
	for(byte i = 0 ; i <= 3; i++) {
		digitalWrite(ledPins[i], 0);
		digitalWrite(ledPins[i+1], 1);
		tone(SPEAKER, notes[i]*2);
		delay(100);
	}
	noTone(SPEAKER);
	digitalWrite(ledPins[4], 0);
}

void buzzSound() {
	tone(SPEAKER, 220, 200);
}

void quizzWaitStart() {
	if (digitalRead(buttonPins[0]) != 0) {
		return;
	}
	Serial.println("Waiting first push");

	quizzSound();
	digitalWrite(ledPins[0], 1);

	step = QUIZZ_WAIT_FIRST;
}

void quizzWaitFirst() {
	byte b = PINC & 0b11110;
	if (b == 0b11110) {
		return;
	}

	for(byte i = 1, m = 0b00010 ; i <= 4; i++) {
		if ((b & m) == 0) {
			Serial.print("Winner "); Serial.println(i);
			digitalWrite(ledPins[i], 1);
		}
		m <<= 1;
	}
	buzzSound();
	digitalWrite(ledPins[0], 0);
	step = QUIZZ_WAIT_START;
}

void checkLongPress() {
	static byte b0 = 1;
	static long lastChange = -1;

	byte b = PINC & 1;
	if (b != b0) { // button change
		long now = millis();
		Serial.print("B0 changed to ");
		Serial.print(b);
		Serial.print(" at ");
		Serial.println(now);
		if (b == 1 && b0 == 0) { // button release
			if (lastChange != -1 && now - lastChange > LONG_PRESS_DELAY) {
				Serial.println("LONG PRESS RELEASED");
				switch (step) {
				case QUIZZ_WAIT_START:
				case QUIZZ_WAIT_FIRST:
					step = SIMON_TODO;
					break;
				case SIMON_TODO:
					step = QUIZZ_WAIT_START;
					break;
				}
			}
		}
		b0 = b;
		lastChange = now;
	}
}

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);
	Serial.println("Init");

	for(int i = 0 ; i <= 4; i++) {
		pinMode(buttonPins[i], INPUT_PULLUP);
		pinMode(ledPins[i], OUTPUT);
	}

	quizzSound();
	step = QUIZZ_WAIT_START;

	Serial.println("Ready.");
}

void loop() {
	checkLongPress();

	switch (step) {
	case QUIZZ_WAIT_START:
		quizzWaitStart();
		break;
	case QUIZZ_WAIT_FIRST:
		quizzWaitFirst();
		break;
	}
}
