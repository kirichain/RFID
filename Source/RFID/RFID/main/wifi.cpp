//
// Created by Tan on 25-Oct-23.
//

#include "wifi.h"

Wifi::Wifi() {
    currentApWifiSSID = nullptr;
    currentApWifiPassword = nullptr;
    currentStaWifiSSID = nullptr;
    currentStaWifiPassword = nullptr;
}

bool Wifi::init_ap_mode() {
    return true;
}

bool Wifi::init_sta_mode() {
    return true;
}

bool Wifi::init_mdns() {
    return true;
}

void Wifi::init_web_page() {

}

void Wifi::set_ap_wifi_credential(char* ssid, char* password) {
    currentApWifiSSID = ssid;
    currentApWifiPassword = password;
    Serial.println(F("Set new AP Wifi credential"));
}

void Wifi::set_sta_wifi_credential(char * ssid, char * password) {
    currentStaWifiSSID = ssid;
    currentStaWifiPassword = password;
    Serial.println(F("Set new STA Wifi credential"));
}

bool Wifi::terminate_ap_mode() {
    return true;
}

bool Wifi::terminate_sta_mode() {
    return true;
}

bool Wifi::terminate_web_page() {
    return true;
}