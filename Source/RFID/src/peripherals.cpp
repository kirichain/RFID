//
// Created by Tan on 25-Oct-23.
//
#include "peripherals.h"

Peripherals::Peripherals() {
    lastLeftUpNavButtonState = 0;
    lastBackCancelNavButtonState = 0;
    lastMenuSelectNavButtonState = 0;
    lastRightDownNavButtonState = 0;

    leftUpNavButtonPin = 0;
    backCancelNavButtonPin = 0;
    menuSelectNavButtonPin = 0;
    rightDownNavButtonPin = 0;
}

void Peripherals::init_navigation_buttons(byte _leftUpNavButtonPin, byte _backCancelNavButtonPin,
                                          byte _menuSelectNavButtonPin, byte _rightDownNavButtonPin) {
    leftUpNavButtonPin = _leftUpNavButtonPin;
    backCancelNavButtonPin = _backCancelNavButtonPin;
    menuSelectNavButtonPin = _menuSelectNavButtonPin;
    rightDownNavButtonPin = _rightDownNavButtonPin;

    pinMode(leftUpNavButtonPin, INPUT_PULLUP);
    pinMode(backCancelNavButtonPin, INPUT_PULLUP);
    pinMode(menuSelectNavButtonPin, INPUT_PULLUP);
    pinMode(rightDownNavButtonPin, INPUT_PULLUP);
    Serial.println("Initialized navigation buttons");
}

bool Peripherals::read_navigation_buttons(byte &currentScreenItemIndex, byte &screenItemCount,
                                          feature_item_type_t &feature_item_type) {
    byte currentLeftUpNavButtonState = digitalRead(leftUpNavButtonPin);
    byte currentBackCancelNavButtonState = digitalRead(backCancelNavButtonPin);
    byte currentMenuSelectNavButtonState = digitalRead(menuSelectNavButtonPin);
    byte currentRightDownNavButtonState = digitalRead(rightDownNavButtonPin);

    // Check for left up navigation button press
    if (currentLeftUpNavButtonState != lastLeftUpNavButtonState) {
        if (currentLeftUpNavButtonState == LOW) {
            Serial.println(F("Left Up Navigation Button Pressed"));
            if (currentScreenItemIndex > 0) {
                --currentScreenItemIndex;
            } else {
                currentScreenItemIndex = screenItemCount - 1;
            }
            Serial.print(F("Current screen item index changed to : "));
            Serial.println(currentScreenItemIndex);
        }
        lastLeftUpNavButtonState = currentLeftUpNavButtonState;
        delay(50); // Delay for debouncing
        return true;
    }

    // Check for back cancel navigation button press
    if (currentBackCancelNavButtonState != lastBackCancelNavButtonState) {
        if (currentBackCancelNavButtonState == LOW) {
            Serial.println(F("Back Cancel Navigation Button Pressed"));
        }
        lastBackCancelNavButtonState = currentBackCancelNavButtonState;
        delay(50); // Delay for debouncing
        return true;
    }

    // Check for menu select navigation button press
    if (currentMenuSelectNavButtonState != lastMenuSelectNavButtonState) {
        if (currentMenuSelectNavButtonState == LOW) {
            Serial.println(F("Menu Select Navigation Button Pressed"));
        }
        lastMenuSelectNavButtonState = currentMenuSelectNavButtonState;
        delay(50); // Delay for debouncing
        return true;
    }

    // Check for right down navigation button press
    if (currentRightDownNavButtonState != lastRightDownNavButtonState) {
        if (currentRightDownNavButtonState == LOW) {
            Serial.println(F("Right Down Navigation Button Pressed"));
            if (currentScreenItemIndex < screenItemCount - 1) {
                ++currentScreenItemIndex;
            } else {
                currentScreenItemIndex = 0;
            }
            Serial.print(F("Current screen item index changed to : "));
            Serial.println(currentScreenItemIndex);
        }
        lastRightDownNavButtonState = currentRightDownNavButtonState;
        delay(50); // Delay for debouncing
        return true;
    }
    return false;
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