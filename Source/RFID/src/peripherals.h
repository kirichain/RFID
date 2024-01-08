//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_PERIPHERALS_H
#define RFID_PERIPHERALS_H

#include "Arduino.h"
#include "enums.h"

class Peripherals {
private:
    byte leftUpNavButtonPin, backCancelNavButtonPin, menuSelectNavButtonPin, rightDownNavButtonPin;
    byte lastMenuSelectNavButtonState, lastLeftUpNavButtonState, lastRightDownNavButtonState, lastBackCancelNavButtonState;
public:
    Peripherals();

    void init_navigation_buttons(byte _leftUpNavButtonPin, byte _backCancelNavButtonPin, byte _menuSelectNavButtonPin,
                                 byte _rightDownNavButtonPin);

    bool read_navigation_buttons(byte &currentScreenItemIndex, byte &screenItemCount,
                                 feature_item_type_t &feature_item_type);

    static void blink_led(byte ledPin);

    void set_digital_input(byte pin);

    static void set_digital_output(byte pin);

    void retrieve_corresponding_task(task_t &previousTask, task_t &currentTask);

    void retrieve_corresponding_feature(feature_t &previousFeature, feature_t &currentFeature);
};

#endif //RFID_PERIPHERALS_H
