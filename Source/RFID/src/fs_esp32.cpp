//
// Created by lenovo on 3/19/2024.
//

#include "fs_esp32.h"

FS32::FS32() {

}

void FS32::save_to_file() {

}

void FS32::read_file() {

}

void FS32::init_spiffs() {
    if (!SPIFFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS-------------------------------");
    }
}
