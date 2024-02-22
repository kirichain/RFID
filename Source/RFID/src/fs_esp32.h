//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_FS_ESP32_H
#define RFID_FS_ESP32_H

#include "Arduino.h"
#include "fs_esp32.h"
//#include "LittleFS.h"
//#include "FS.h"
#include "enums.h"
#include "structs.h"

class FsEsp32 {
public:
    bool isFsLoaded;

    operating_mode_t savedOperatingModeInFs;
    device_config deviceConfig;
    char* fileName;

    FsEsp32();

    bool load_fs();

    bool save_fs();

    bool load_config();

    bool save_config();

//    bool write_to_file(char* fileName, byte mode);
//
//    bool read_file(char* fileName, byte mode);
};

#endif //RFID_FS_ESP32_H
