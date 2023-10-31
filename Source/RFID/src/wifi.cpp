//
// Created by Tan on 25-Oct-23.
//

#include "wifi.h"

char* currentAPWifiSSID = nullptr;
char* currentAPWifipassword = nullptr;
char* currentSTAWifiSSID = nullptr;
char* currentSTAWifipassword = nullptr;

Wifi::Wifi() {

}

bool Wifi::init_ap_mode() {

}

bool Wifi::init_sta_mode() {

}

bool Wifi::init_mdns() {

}

void Wifi::init_web_page() {

}

void Wifi::set_ap_wifi_credential(char* ssid, char* password) {
    currentAPWifiSSID = ssid;
    currentAPWifipassword = password;
    Serial.println(F("Set new AP Wifi credential"));
}

void Wifi::set_sta_wifi_credential(char * ssid, char * password) {
    currentSTAWifiSSID = ssid;
    currentSTAWifipassword = password;
    Serial.println(F("Set new STA Wifi credential"));
}

bool Wifi::terminate_ap_mode() {

}

bool Wifi::terminate_sta_mode() {

}

bool Wifi::terminate_web_page() {

}