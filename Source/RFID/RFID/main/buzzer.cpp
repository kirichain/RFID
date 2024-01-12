//
// Created by Tan on 25-Oct-23.
//

#include "buzzer.h"

Buzzer::Buzzer() {
    Serial.println(F("Buzzer initiated"));
}

void Buzzer::init(byte _buzzerPin) {
    buzzerPin = _buzzerPin;
}

void Buzzer::welcome_sound() const {
    Serial.println(F("Start playing welcome sound"));
    digitalWrite(buzzerPin, 0);
}


