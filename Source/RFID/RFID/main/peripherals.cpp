//
// Created by Tan on 25-Oct-23.
//
#include "peripherals.h"

Peripherals::Peripherals() {
    lastMenuSelectNavButtonState = 0;
    lastLeftUpNavButtonState = 0;
    lastRightDownNavButtonState = 0;
    lastBackCancelNavButtonState = 0;

    menuSelectNavButtonPin = 0;
    leftUpNavButtonPin = 0;
    rightDownNavButtonPin = 0;
    backCancelNavButtonPin = 0;
}

void Peripherals::init_navigation_buttons(byte _menuSelectNavButtonPin, byte _leftUpNavButtonPin,
                                          byte _rightDownNavButtonPin, byte _backCancelNavButtonPin) {
    menuSelectNavButtonPin = _menuSelectNavButtonPin;
    leftUpNavButtonPin = _leftUpNavButtonPin;
    rightDownNavButtonPin = _rightDownNavButtonPin;
    backCancelNavButtonPin = _backCancelNavButtonPin;

    pinMode(menuSelectNavButtonPin, INPUT);
    pinMode(leftUpNavButtonPin, INPUT);
    pinMode(rightDownNavButtonPin, INPUT);
    pinMode(backCancelNavButtonPin, INPUT);
    Serial.println("Initialized navigation buttons");
}

void Peripherals::read_navigation_buttons() {
    lastMenuSelectNavButtonState = 0;
    lastLeftUpNavButtonState = 0;
    lastRightDownNavButtonState = 0;
    lastBackCancelNavButtonState = 0;

    lastMenuSelectNavButtonState = digitalRead(menuSelectNavButtonPin);
    lastLeftUpNavButtonState = digitalRead(leftUpNavButtonPin);
    lastRightDownNavButtonState = digitalRead(rightDownNavButtonPin);
    lastBackCancelNavButtonState = digitalRead(backCancelNavButtonPin);

    Serial.print("Navigation buttons state values: ");
    Serial.print(lastMenuSelectNavButtonState);
    Serial.print(" ,");
    Serial.print(lastLeftUpNavButtonState);
    Serial.print(" ,");
    Serial.print(lastRightDownNavButtonState);
    Serial.print(" ,");
    Serial.print(lastBackCancelNavButtonState);
    Serial.println(".");
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

void Peripherals::retrieve_corresponding_task(task_t &previousTask, task_t &currentTask) {
    previousTask = currentTask;
}

void Peripherals::retrieve_corresponding_feature(feature_t &previousFeature, feature_t &currentFeature) {
    previousFeature = currentFeature;
}