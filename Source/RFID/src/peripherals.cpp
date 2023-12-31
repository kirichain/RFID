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

button_type_t Peripherals::read_navigation_buttons(byte &currentScreenItemIndex, byte &screenItemCount,
                                                   feature_item_type_t &feature_item_type) {
    button_type_t button_type = NOT_PRESSED;

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

            button_type = LEFT_UP;
        }
        lastLeftUpNavButtonState = currentLeftUpNavButtonState;
        delay(50); // Delay for debouncing
    }

    // Check for back cancel navigation button press
    if (currentBackCancelNavButtonState != lastBackCancelNavButtonState) {
        if (currentBackCancelNavButtonState == LOW) {
            Serial.println(F("Back Cancel Navigation Button Pressed"));

            button_type = BACK_CANCEL;
        }
        lastBackCancelNavButtonState = currentBackCancelNavButtonState;
        delay(50); // Delay for debouncing
    }

    // Check for menu select navigation button press
    if (currentMenuSelectNavButtonState != lastMenuSelectNavButtonState) {
        if (currentMenuSelectNavButtonState == LOW) {
            Serial.println(F("Menu Select Navigation Button Pressed"));

            button_type = SELECT;
        }
        lastMenuSelectNavButtonState = currentMenuSelectNavButtonState;
        delay(50); // Delay for debouncing
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

            button_type = RIGHT_DOWN;
        }
        lastRightDownNavButtonState = currentRightDownNavButtonState;
        delay(50); // Delay for debouncing
    }
    return button_type;
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

void Peripherals::retrieve_corresponding_feature(feature_t &previousFeature, feature_t &currentFeature,
                                                 feature_t &argsFeature, byte &screenItemIndex,
                                                 feature_t (&screenFeatures)[10]) {
    previousFeature = currentFeature;
    Serial.print(F("New screen feature index: "));
    Serial.println(screenFeatures[screenItemIndex]);
    argsFeature = screenFeatures[screenItemIndex];
}