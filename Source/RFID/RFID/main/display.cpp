//
// Created by Tan on 25-Oct-23.
//

#include "display.h"

TFT_eSPI tft = TFT_eSPI();

Display::Display() {

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(random(0xFFFF));
}

void Display::render_feature(feature_t _feature) {

}

void Display::blink_screen() {
    while (true) {
        tft.fillScreen(random(0xFFFF));
        delay(1000);
        tft.fillScreen(0xFFFF);
        delay(1000);
    }
}