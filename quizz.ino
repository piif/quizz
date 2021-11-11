#include <Arduino.h>

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define SPEAKER 7
#define LONG_PRESS_DELAY 1000
#define BUZZ_NOTE 220

#define MAX_SONG 25
#define SIMON_PLAY_DELAY 300
#define SIMON_SILENCE_DELAY 200

// Si b , Ré b , Fa , Si b
const int notes[4] = { 466, 554, 697, 932 };

const byte buttonPins[5] = { A0, A1, A2, A3, A4 };
const byte ledPins[5] = { 8, 9, 10, 11, 12 };

enum {
	QUIZZ_INIT,
	QUIZZ_WAIT_START,
	QUIZZ_WAIT_FIRST,
	SIMON_INIT,
	SIMON_WAIT_START,
	SIMON_PLAY,
	SIMON_LISTEN_WAIT_PRESS,
	SIMON_LISTEN_WAIT_RELEASE
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

void quizzInit() {
	digitalWrite(ledPins[0], 0);
	quizzSound();

	Serial.println("Waiting start");
	step = QUIZZ_WAIT_START;
}

void quizzWaitStart() {
	if (digitalRead(buttonPins[0]) != 0) {
		return;
	}

	quizzSound();
	digitalWrite(ledPins[0], 1);

	Serial.println("Waiting first push");
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
	tone(SPEAKER, BUZZ_NOTE);
	delay(200);
	noTone(SPEAKER);

	digitalWrite(ledPins[0], 0);
	step = QUIZZ_WAIT_START;
}

byte songLast;
byte song[MAX_SONG];
byte playIndex;

void simonAddNote() {
	song[songLast] = random(0,4) + 1;
	Serial.print("Add "); Serial.println(song[songLast]);
	songLast++;
}

void simonSound() {
	for(byte i = 0 ; i <= 3; i++) {
		digitalWrite(ledPins[i], 0);
		digitalWrite(ledPins[i+1], 1);
		tone(SPEAKER, notes[i]);
		delay(200);
	}
	noTone(SPEAKER);
	digitalWrite(ledPins[4], 0);
}

void simonInit() {
	digitalWrite(ledPins[0], 0);

	simonSound();
	step = SIMON_WAIT_START;
}

void simonWaitStart() {
	if (digitalRead(buttonPins[0]) != 0) {
		return;
	}

	randomSeed(analogRead(A5));
	songLast=0;

	simonAddNote();
	step = SIMON_PLAY;
}

void simonPlay() {
	digitalWrite(ledPins[0], 0);

	for(byte i = 0; i < songLast; i++) {
		digitalWrite(ledPins[song[i]], 1);
		tone(SPEAKER, notes[song[i]-1]);
		delay(SIMON_PLAY_DELAY);

		digitalWrite(ledPins[song[i]], 0);
		noTone(SPEAKER);
		delay(SIMON_SILENCE_DELAY);
	}

	playIndex = 0;
	digitalWrite(ledPins[0], 1);
	step = SIMON_LISTEN_WAIT_PRESS;
}

// get buttons state, wait for bump, return FF if nothing changed, or new button state
byte buttonChange() {
	static byte buttons = 0b11110;

	byte b = PINC & 0b11110;
	if (b == buttons) {
		return 0xFF;
	}
	delay(3);
	b = PINC & 0b11110;
	if (b == buttons) {
		return 0xFF;
	}
	buttons = b;
	Serial.print("Buttons = "); Serial.println(buttons , BIN);
	return buttons;
}

void simonListenWaitPress() {
	byte b = buttonChange();
	if (b == 0xFF) {
		return;
	}

	byte n;
	switch(b) {
	case 0b11100:
		n=1;
		break;
	case 0b11010:
		n=2;
		break;
	case 0b10110:
		n=3;
		break;
	case 0b01110:
		n=4;
		break;
	default:
		n=0xFF;
	}

	Serial.print("Press "); Serial.print(n); Serial.print(", wait "); Serial.println(song[playIndex]);

	if (song[playIndex] == n) {
		digitalWrite(ledPins[n], 1);
		tone(SPEAKER, notes[song[playIndex]-1]);
		delay(SIMON_PLAY_DELAY);
		digitalWrite(ledPins[n], 0);
		noTone(SPEAKER);
		playIndex++;

		step = SIMON_LISTEN_WAIT_RELEASE;

	} else {
		if (n != 0xFF) {
			digitalWrite(ledPins[n], 1);
		}
		tone(SPEAKER, BUZZ_NOTE);
		delay(SIMON_PLAY_DELAY*2);
		if (n != 0xFF) {
			digitalWrite(ledPins[n], 0);
		}
		noTone(SPEAKER);

		step = SIMON_INIT;
	}
}

void simonListenWaitRelease() {
	byte b = buttonChange();
	if (b == 0xFF) {
		return;
	}
	if (b == 0b11110) {
		if (playIndex == songLast) {
			if (playIndex == MAX_SONG) {
				simonSound();
				simonSound();
				step = SIMON_INIT;
			} else {
				simonAddNote();
				delay(SIMON_PLAY_DELAY*2);
				step = SIMON_PLAY;
			}
		} else {
			step = SIMON_LISTEN_WAIT_PRESS;
		}
	}
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
				case QUIZZ_INIT:
				case QUIZZ_WAIT_START:
				case QUIZZ_WAIT_FIRST:
					step = SIMON_INIT;
					break;
				case SIMON_INIT:
				case SIMON_WAIT_START:
				case SIMON_PLAY:
				case SIMON_LISTEN_WAIT_PRESS:
				case SIMON_LISTEN_WAIT_RELEASE:
					step = QUIZZ_INIT;
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

	step = QUIZZ_INIT; // SIMON_INIT;

	Serial.println("Ready.");
}

void loop() {
	checkLongPress();

	switch (step) {
	case QUIZZ_INIT:
		quizzInit();
		break;
	case QUIZZ_WAIT_START:
		quizzWaitStart();
		break;
	case QUIZZ_WAIT_FIRST:
		quizzWaitFirst();
		break;

	case SIMON_INIT:
		simonInit();
		break;
	case SIMON_WAIT_START:
		simonWaitStart();
		break;
	case SIMON_PLAY:
		simonPlay();
		break;
	case SIMON_LISTEN_WAIT_PRESS:
		simonListenWaitPress();
		break;
	case SIMON_LISTEN_WAIT_RELEASE:
		simonListenWaitRelease();
		break;
	}
}
