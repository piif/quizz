#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define WITH_TRANSISTOR

long tic;

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

	pinMode(13, OUTPUT);
	digitalWrite(13, 0);
	Serial.println("Ready.");
	tic=millis();
}

byte button = 1;
byte led = 0;
long cpt = 0;

void loop() {

	cpt++;
	long now=millis();
	if (now-tic > 1000) {
		led = !led;
		Serial.print("switch "); Serial.print(led); Serial.print(" at "); Serial.println(cpt);
		tic = now;
		cpt=0;
	}

#ifndef WITH_TRANSISTOR
	// led
	pinMode(3, INPUT_PULLUP);
	digitalWrite(2, 0);
	byte b = digitalRead(3);
	if (b!=button) {
		button = b;
		Serial.print("button "); Serial.println(button);
	}

	// button
	pinMode(3, OUTPUT);
	digitalWrite(3, 0);
	digitalWrite(2, led);
#endif

#ifdef WITH_TRANSISTOR
	// led
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, INPUT);

	digitalWrite(2, 0);
	digitalWrite(3, !led);

	// button
	pinMode(3, OUTPUT);
	digitalWrite(3, 1);
	pinMode(4, OUTPUT);
	digitalWrite(4, 0);

	pinMode(2, INPUT_PULLUP);
#endif

	byte b = digitalRead(2);
	if (b!=button) {
		button = b;
		Serial.print("button "); Serial.println(button);
		digitalWrite(13, !button);

	}
}
