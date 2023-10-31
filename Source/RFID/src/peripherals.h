//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_PERIPHERALS_H
#define RFID_PERIPHERALS_H

#include "Arduino.h"
#include "enums.h"

class Peripherals {
public:
    byte lastMenuSelectNavButton;
    byte lastLeftUpNavButton;
    byte lastRightDownNavButton;

    Peripherals();

    void init_navigation_buttons(byte _menuSelectNavButton, byte _leftUpNavButton, byte _rightDownNavButton);

    void read_navigation_buttons(byte _menuSelectNavButton, byte _leftUpNavButton, byte _rightDownNavButton);

    task retrieve_corresponding_task(task _currentTask);

    feature retrieve_corresponding_feature(feature _currentFeature);
};

#endif //RFID_PERIPHERALS_H
