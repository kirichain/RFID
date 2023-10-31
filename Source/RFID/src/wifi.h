//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_WIFI_H
#define RFID_WIFI_H

#include "Arduino.h"

extern char* currentAPWifiSSID;
extern char* currentAPWifipassword;
extern char* currentSTAWifiSSID;
extern char* currentSTAWifipassword;

class Wifi {
public:
    Wifi();

    bool init_ap_mode();

    bool init_sta_mode();

    bool init_mdns();

    void init_web_page();

    void set_ap_wifi_credential(char * ssid, char * password);

    void set_sta_wifi_credential(char * ssid, char * password);

    bool terminate_ap_mode();

    bool terminate_sta_mode();

    bool terminate_web_page();
};
#endif //RFID_WIFI_H
