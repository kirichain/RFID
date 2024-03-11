//
// Created by Tan on 25-Oct-23.
//
#include "peripherals.h"

volatile bool Peripherals::isMenuSelectButtonReleased = false;
volatile bool Peripherals::isMenuSelectButtonPressed = false;

byte Peripherals::menuSelectNavButtonPin = 0;
byte Peripherals::lastMenuSelectNavButtonState = 0;

unsigned long Peripherals::lastSelectButtonDebounceTime = 0;

void IRAM_ATTR select_button_isr() {
    unsigned long currentTime = millis();
    byte currentButtonState = digitalRead(Peripherals::menuSelectNavButtonPin);
    if (currentTime - Peripherals::lastSelectButtonDebounceTime >= DEBOUNCE_DELAY) {
        // If the button state has changed
        if (currentButtonState != Peripherals::lastMenuSelectNavButtonState) {
            Peripherals::lastMenuSelectNavButtonState = currentButtonState;
            Peripherals::lastSelectButtonDebounceTime = currentTime;

            // Update flags based on the current state of the button
            if (digitalRead(Peripherals::menuSelectNavButtonPin) == LOW) {
                Peripherals::isMenuSelectButtonPressed = true;
            } else {
                Peripherals::isMenuSelectButtonReleased = true;
            }
        }
    }
}

Peripherals::Peripherals() {
    lastLeftUpNavButtonState = 0;
    lastBackCancelNavButtonState = 0;
//    lastMenuSelectNavButtonState = 0;
    lastRightDownNavButtonState = 0;

    leftUpNavButtonPin = 0;
    backCancelNavButtonPin = 0;
    rightDownNavButtonPin = 0;
}

void Peripherals::init_navigation_buttons(byte _leftUpNavButtonPin, byte _backCancelNavButtonPin,
                                          byte _menuSelectNavButtonPin, byte _rightDownNavButtonPin) {
    leftUpNavButtonPin = _leftUpNavButtonPin;
    backCancelNavButtonPin = _backCancelNavButtonPin;
    Serial.println(F("Back button is "));
    Serial.println(backCancelNavButtonPin);
    Peripherals::menuSelectNavButtonPin = _menuSelectNavButtonPin;
    rightDownNavButtonPin = _rightDownNavButtonPin;

    pinMode(leftUpNavButtonPin, INPUT_PULLUP);
    pinMode(backCancelNavButtonPin, INPUT_PULLUP);
    pinMode(Peripherals::menuSelectNavButtonPin, INPUT_PULLUP);
    pinMode(rightDownNavButtonPin, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(Peripherals::menuSelectNavButtonPin), select_button_isr, CHANGE);
    Serial.println("Initialized navigation buttons");
}

button_type_t Peripherals::read_navigation_buttons(byte &currentScreenItemIndex, byte &screenItemCount,
                                                   feature_item_type_t &feature_item_type) {
    button_type_t button_type = NOT_PRESSED;

    byte currentLeftUpNavButtonState = digitalRead(leftUpNavButtonPin);
    byte currentBackCancelNavButtonState = digitalRead(backCancelNavButtonPin);
    //byte currentMenuSelectNavButtonState = digitalRead(Peripherals::menuSelectNavButtonPin);
    byte currentRightDownNavButtonState = digitalRead(rightDownNavButtonPin);

    // Check if the left up navigation button is pressed
    if (currentLeftUpNavButtonState != lastLeftUpNavButtonState) {
        if (currentLeftUpNavButtonState == LOW) {
            Serial.println(F("Left Up Navigation Button Has Been Pressed"));
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

    // Check if back cancel navigation button is pressed
    if (currentBackCancelNavButtonState != lastBackCancelNavButtonState) {
        if (currentBackCancelNavButtonState == LOW) {
            Serial.println(F("Back Cancel Navigation Button Has Been Pressed"));

            button_type = BACK_CANCEL;
        }
        lastBackCancelNavButtonState = currentBackCancelNavButtonState;
        delay(50); // Delay for debouncing
    }

    //Check if the menu select navigation button is pressed or released
    if (isMenuSelectButtonPressed) {
        Serial.println(F("Menu Select Navigation Button Has Been Pressed"));
        button_type = SELECT;

        isMenuSelectButtonPressed = false;
    } else if (isMenuSelectButtonReleased) {
        Serial.println(F("Menu Select Navigation Button Has Been Released"));
        button_type = NOT_PRESSED;

        isMenuSelectButtonReleased = false;
    }

//    if (currentMenuSelectNavButtonState != lastMenuSelectNavButtonState) {
//        if (currentMenuSelectNavButtonState == LOW) {
//            Serial.println(F("Menu Select Navigation Button Has Been Pressed"));
//
//            button_type = SELECT;
//        }
//        lastMenuSelectNavButtonState = currentMenuSelectNavButtonState;
//        delay(50); // Delay for debouncing
//    } else {
//        // Button is being pressed, return state SELECT
//        if (currentMenuSelectNavButtonState == LOW) {
//            button_type = SELECT;
//        }
//        delay(50); // Delay for debouncing
//    }

    // Check if right down navigation button is pressed
    if (currentRightDownNavButtonState != lastRightDownNavButtonState) {
        if (currentRightDownNavButtonState == LOW) {
            Serial.println(F("Right Down Navigation Button Has Been Pressed"));
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
                                                 feature_t (&screenFeatures)[10], button_type_t &button_type,
                                                 feature_t (&navigation_history)[10],
                                                 byte &navigation_history_size) {
    switch (button_type) {
        case SELECT:
            previousFeature = currentFeature;
            Serial.print(F("New screen feature index: "));
            Serial.println(screenFeatures[screenItemIndex]);
            argsFeature = screenFeatures[screenItemIndex];
            if (navigation_history_size < 10) {
                navigation_history[++navigation_history_size] = argsFeature;
            } else {

            }
            break;
        case BACK_CANCEL:
            if (navigation_history_size > 1) {
                argsFeature = previousFeature;
                Serial.println(F("Back to previous feature"));
                navigation_history[navigation_history_size] = NO_FEATURE;
                --navigation_history_size;
                if (navigation_history[navigation_history_size - 1] != NO_FEATURE) {
                    previousFeature = navigation_history[navigation_history_size - 1];
                } else {
                    // We are in home already
                }
            } else {

            }
            break;
    }

}