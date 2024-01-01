//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_DISPLAY_H
#define RFID_DISPLAY_H

#include "Arduino.h"
#include "enums.h"
#include "SPI.h"
#include "TFT_eSPI.h"

class Display {
public:
    Display();

    void render_feature(feature_t _feature);
    void blink_screen();
};

#endif //RFID_DISPLAY_H
