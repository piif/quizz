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

// // l'état des boutons
// byte b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0;

// // l'état des lumières
// byte l0 = 0, l1 = 0, l2 = 0, l3 = 0, l4 = 0;

// byte state;

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

    // tout éteindre
    digitalWrite(LED_0, LOW);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);

    // attendre un gagnant
    attendreGagnant();

    // b0 = !digitalRead(BOUTON_0);
    // b1 = !digitalRead(BOUTON_1);
    // b2 = !digitalRead(BOUTON_2);
    // b3 = !digitalRead(BOUTON_3);
    // b4 = !digitalRead(BOUTON_4);

    // byte newState = (b4 << 4) | (b3 << 3) | (b2 << 2) | (b1 << 1) | b0;
    // if (state != newState) {
    //     state = newState;
    //     Serial.println(state, BIN);
    // }

    // digitalWrite(LED_0, b0);
    // digitalWrite(LED_1, b1);
    // digitalWrite(LED_2, b2);
    // digitalWrite(LED_3, b3);
    // digitalWrite(LED_4, b4);
}
