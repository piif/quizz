#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define BUTTON_0 2

#define LED_0 13

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(LED_0, OUTPUT);

	pinMode(BUTTON_0, INPUT_PULLUP);

	Serial.println("Ready.");
}

byte button = 1;
byte led;

void loop() {
	byte b;
	b = digitalRead(BUTTON_0);
	if (b != button) {
		delay(2);
		b = digitalRead(BUTTON_0);
		if (b != button) {
			button = b;
			Serial.print("button : "); Serial.println(b);
			led = !b;
			digitalWrite(LED_0, led);
		}
	}
}
