//
// Created by Tan on 25-Oct-23.
//
#include "peripherals.h"

Peripherals::Peripherals() {
    lastMenuSelectNavButton = 0;
    lastLeftUpNavButton = 0;
    lastRightDownNavButton = 0;
}

void Peripherals::init_navigation_buttons(byte _menuSelectNavButton, byte _leftUpNavButton, byte _rightDownNavButton) {
    pinMode(_menuSelectNavButton, INPUT);
    pinMode(_leftUpNavButton, INPUT);
    pinMode(_rightDownNavButton, INPUT);

    Serial.println("Initialzed Navigation Buttons");
}

void Peripherals::read_navigation_buttons(byte _menuSelectNavButton, byte _leftUpNavButton, byte _rightDownNavButton) {
    lastMenuSelectNavButton = 0;
    lastLeftUpNavButton = 0;
    lastRightDownNavButton = 0;

    lastMenuSelectNavButton = digitalRead(_menuSelectNavButton);
    lastLeftUpNavButton = digitalRead(_leftUpNavButton);
    lastRightDownNavButton = digitalRead(_rightDownNavButton);

    Serial.print("Naviation buttons state values: ");
    Serial.print(lastMenuSelectNavButton);
    Serial.print(" ,");
    Serial.print(lastLeftUpNavButton);
    Serial.print(" ,");
    Serial.print(lastRightDownNavButton);
    Serial.println(" .");

}

task Peripherals::retrieve_corresponding_task(task _currentTask) {

    return _currentTask;
}

feature Peripherals::retrieve_corresponding_feature(feature _currentFeature) {
    return _currentFeature;
}