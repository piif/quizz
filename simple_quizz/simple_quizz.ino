#define BOUTON_0 8
#define BOUTON_1 9
#define BOUTON_2 10
#define BOUTON_3 11
#define BOUTON_4 12

#define LED_0 3
#define LED_1 4
#define LED_2 5
#define LED_3 6
#define LED_4 7

#define BUZZER A0

// Si b , Ré b , Fa , Si b
#define NOTE_1 466
#define NOTE_2 554
#define NOTE_3 697
#define NOTE_4 932
#define NOTE_BUZZ 220

void setup() {
    Serial.begin(115200);
    pinMode(BOUTON_0, INPUT_PULLUP);
    pinMode(BOUTON_1, INPUT_PULLUP);
    pinMode(BOUTON_2, INPUT_PULLUP);
    pinMode(BOUTON_3, INPUT_PULLUP);
    pinMode(BOUTON_4, INPUT_PULLUP);

    pinMode(LED_0, OUTPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);
    pinMode(LED_4, OUTPUT);

    // tout éteindre
    digitalWrite(LED_0, LOW);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);

    pinMode(BUZZER, OUTPUT);

    Serial.println("fin setup");
}

void buzz() {
	tone(BUZZER, NOTE_BUZZ);
	delay(200);
	noTone(BUZZER);
}

void jingle() {
	tone(BUZZER, NOTE_1);
	delay(100);
	tone(BUZZER, NOTE_2);
	delay(100);
	tone(BUZZER, NOTE_3);
	delay(100);
	tone(BUZZER, NOTE_4);
	delay(100);
	noTone(BUZZER);
}

void attendreDepart() {
    while(digitalRead(BOUTON_0) == HIGH) {
    }
    Serial.println("depart !");
}

void attendreGagnant() {
    while(true) {
        if (digitalRead(BOUTON_1) == LOW) {
            digitalWrite(LED_1, HIGH);
            Serial.println("1 gagne");
            return;
        } else if (digitalRead(BOUTON_2) == LOW) {
            digitalWrite(LED_2, HIGH);
            Serial.println("2 gagne");
            return;
        } else if (digitalRead(BOUTON_3) == LOW) {
            digitalWrite(LED_3, HIGH);
            Serial.println("3 gagne");
            return;
        } else if (digitalRead(BOUTON_4) == LOW) {
            digitalWrite(LED_4, HIGH);
            Serial.println("4 gagne");
            return;
        }
    }
}

void loop() {
    // allumer 0
    digitalWrite(LED_0, HIGH);

    // attendre le lancement de partie
    attendreDepart();
    jingle();

    // tout éteindre
    digitalWrite(LED_0, LOW);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);

    // attendre un gagnant
    attendreGagnant();
    buzz();
}
