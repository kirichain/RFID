//
// Created by Tan on 25-Oct-23.
//

#include "fs_esp32.h"

FsEsp32::FsEsp32() {
    isFsLoaded = false;
    deviceConfig.defaultApWifiPassword = nullptr;
    deviceConfig.defaultApWifiSSID = nullptr;
    deviceConfig.defaultStaWifiSSID = nullptr;
    deviceConfig.defaultStaWifiPassword = nullptr;
    fileName = nullptr;
}

bool FsEsp32::load_fs() {
//    if (read_file(fileName, 'w')) {
//        isFsLoaded = true;
//        Serial.println(F("FS is loaded successfully"));
//    } else {
//        Serial.println(F("FS is not loaded"));
//        isFsLoaded = false;
//    }
    return isFsLoaded;
}

bool FsEsp32::save_fs() {
//    if (write_to_file(fileName, 'w')) {
//        Serial.println(F("Saved FS"));
//        return true;
//    } else {
//        Serial.println(F("FS saving is not completed"));
//        return false;
//    }
return true;
}

bool FsEsp32::load_config() {
    return true;
}

bool FsEsp32::save_config() {
    return true;
}

//bool FsEsp32::write_to_file(char *fileName, byte mode) {
//    return true;
//}
//
//bool FsEsp32::read_file(char *fileName, byte mode) {
//    return true;
//}
