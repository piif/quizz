#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

const byte buttonPins[5] = { 2, 3, 4, 5, 6 };
const byte ledPins[5] = { 13, 9, 10, 11, 12 };

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	for(int i = 0 ; i <= 4; i++) {
		pinMode(buttonPins[i], INPUT_PULLUP);
		pinMode(ledPins[i], OUTPUT);
	}

	Serial.println("Ready.");
}

byte buttons[5] = { 1, 1, 1, 1, 1 };
byte leds[5]; // 0 partout par défaut

void loop() {
	for(int i = 0 ; i <= 4; i++) {
		byte b;
		b = digitalRead(buttonPins[i]);
		if (b != buttons[i]) {
			delay(2);
			b = digitalRead(buttonPins[i]);
			if (b != buttons[i]) {
				buttons[i] = b;
				Serial.print("button "); Serial.print(i); Serial.print(" : "); Serial.println(b);
				leds[i]= !b;
				digitalWrite(ledPins[i], leds[i]);
			}
		}
	}
}
