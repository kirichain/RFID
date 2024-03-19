//
// Created by lenovo on 3/19/2024.
//

#ifndef RFID_FS_ESP32_H
#define RFID_FS_ESP32_H

#include "SPIFFS.h"

class FS32 {
public:

    FS32();

    void save_to_file();

    void read_file();

    void init_spiffs();
};
#endif //RFID_FS_ESP32_H
