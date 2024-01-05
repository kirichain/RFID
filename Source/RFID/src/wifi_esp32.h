//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_WIFI_H
#define RFID_WIFI_H

#include "Arduino.h"
#include <Wifi.h>

class Wifi {
public:
    char *currentApWifiSSID;
    char *currentApWifiPassword;
    char *currentStaWifiSSID;
    char *currentStaWifiPassword;
    char *currentHostname;

    Wifi();

    bool init_ap_mode() const;

    bool init_sta_mode() const;

    bool init_mdns();

    void init_web_page();

    void set_ap_wifi_credential(char *ssid, char *password);

    void set_sta_wifi_credential(char *ssid, char *password, char *hostname);

    static void terminate_ap_mode();

    static void terminate_sta_mode();

    void terminate_web_page();
};

#endif //RFID_WIFI_H
