//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_WIFI_H
#define RFID_WIFI_H

#include "Arduino.h"

class Wifi {
public:
    Wifi();

    bool init_ap_mode();

    bool init_sta_mode();

    bool init_mdns();

    void init_web_page();

    void set_wifi_credential(char * ssid, char * password);
};
#endif //RFID_WIFI_H
