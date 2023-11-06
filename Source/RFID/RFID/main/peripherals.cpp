//
// Created by Tan on 25-Oct-23.
//
#include "peripherals.h"

Peripherals::Peripherals() {
    lastMenuSelectNavButtonState = 0;
    lastLeftUpNavButtonState = 0;
    lastRightDownNavButtonState = 0;
}

void Peripherals::init_navigation_buttons(byte _menuSelectNavButton, byte _leftUpNavButton, byte _rightDownNavButton) {
    pinMode(_menuSelectNavButton, INPUT);
    pinMode(_leftUpNavButton, INPUT);
    pinMode(_rightDownNavButton, INPUT);

    Serial.println("Initialzed Navigation Buttons");
}

void Peripherals::read_navigation_buttons(byte _menuSelectNavButton, byte _leftUpNavButton, byte _rightDownNavButton) {
    lastMenuSelectNavButtonState = 0;
    lastLeftUpNavButtonState = 0;
    lastRightDownNavButtonState = 0;

    lastMenuSelectNavButtonState = digitalRead(_menuSelectNavButton);
    lastLeftUpNavButtonState = digitalRead(_leftUpNavButton);
    lastRightDownNavButtonState = digitalRead(_rightDownNavButton);

    Serial.print("Naviation buttons state values: ");
    Serial.print(lastMenuSelectNavButtonState);
    Serial.print(" ,");
    Serial.print(lastLeftUpNavButtonState);
    Serial.print(" ,");
    Serial.print(lastRightDownNavButtonState);
    Serial.println(" .");

}

void Peripherals::blink_led(byte ledPin) {
    set_digital_output(ledPin);
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
}

void Peripherals::set_digital_input(byte pin) {
    pinMode(pin, INPUT);
    Serial.print(F("Set pin "));
    Serial.print(pin);
    Serial.println(F(" as INPUT"));
}

void Peripherals::set_digital_output(byte pin) {
    pinMode(pin, OUTPUT);
    Serial.print(F("Set pin "));
    Serial.print(pin);
    Serial.println(F(" as OUTPUT"));
}

task_t Peripherals::retrieve_corresponding_task(task_t _currentTask) {

    return _currentTask;
}

feature_t Peripherals::retrieve_corresponding_feature(feature_t _currentFeature) {
    return _currentFeature;
}