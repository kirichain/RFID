//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_PERIPHERALS_H
#define RFID_PERIPHERALS_H

#include "Arduino.h"
#include "enums.h"

class Peripherals {
public:
    byte lastMenuSelectNavButtonState;
    byte lastLeftUpNavButtonState;
    byte lastRightDownNavButtonState;

    Peripherals();

    void init_navigation_buttons(byte _menuSelectNavButton, byte _leftUpNavButton, byte _rightDownNavButton);

    void read_navigation_buttons(byte _menuSelectNavButton, byte _leftUpNavButton, byte _rightDownNavButton);

    void blink_led(byte ledPin);

    void set_digital_input(byte pin);

    void set_digital_output(byte pin);

    task_t retrieve_corresponding_task(task_t _currentTask);

    feature_t retrieve_corresponding_feature(feature_t _currentFeature);
};

#endif //RFID_PERIPHERALS_H
