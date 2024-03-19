//
// Created by lenovo on 3/19/2024.
//

#ifndef RFID_FS_ESP32_H
#define RFID_FS_ESP32_H

#include "SPIFFS.h"
#include <ArduinoJson.h>

class FS32 {
public:
    String ssid = "";
    String password = "";
    String mes_package = "";
    String mes_package_group = "";

    FS32();

    static bool save_settings(String ssid, String password, String mes_package, String mes_package_group);

    bool read_saved_settings();

    static bool init_spiffs();

};

#endif //RFID_FS_ESP32_H
