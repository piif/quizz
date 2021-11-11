#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

const byte buttonPins[5] = { 2, 3, 4, 5, 6 };
const byte ledPins[5] = { 8, 9, 10, 11, 12 };

#define BUTTON_MASK ((PIND & 0b01111100 ) >> 2)

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	for(int i = 0 ; i <= 4; i++) {
		pinMode(buttonPins[i], INPUT_PULLUP);
		pinMode(ledPins[i], OUTPUT);
	}

	Serial.println("Ready.");
}

byte buttons = 0b11111;
byte leds[5]; // 0 partout par défaut

void loop() {
	byte b = BUTTON_MASK;
	if (b != buttons) {
		delay(2);
		b = BUTTON_MASK;
		if (b != buttons) {
			Serial.println(b, BIN);

			for(byte i = 0, m = 1 ; i <= 4; i++) {
				if ((b & m) != (buttons & m)) {
					Serial.print("button "); Serial.print(i); Serial.print(" : "); Serial.println((b & m) ? 1 : 0);
					leds[i]= !(b & m);
					digitalWrite(ledPins[i], leds[i]);
				}
				m <<= 1;
			}

			buttons = b;
		}
	}
}
