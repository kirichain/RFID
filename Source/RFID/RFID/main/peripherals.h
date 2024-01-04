//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_PERIPHERALS_H
#define RFID_PERIPHERALS_H

#include "Arduino.h"
#include "enums.h"

class Peripherals {
private:
    byte menuSelectNavButtonPin, leftUpNavButtonPin, rightDownNavButtonPin, backCancelNavButtonPin;
    byte lastMenuSelectNavButtonState, lastLeftUpNavButtonState, lastRightDownNavButtonState, lastBackCancelNavButtonState;
public:
    Peripherals();

    void init_navigation_buttons(byte _menuSelectNavButtonPin, byte _leftUpNavButtonPin, byte _rightDownNavButtonPin,
                                 byte _backCancelNavButtonPin);

    void read_navigation_buttons();

    void blink_led(byte ledPin);

    void set_digital_input(byte pin);

    void set_digital_output(byte pin);

    void retrieve_corresponding_task(task_t &previousTask, task_t &currentTask);

    void retrieve_corresponding_feature(feature_t &previousFeature, feature_t &currentFeature);
};

#endif //RFID_PERIPHERALS_H
