//
// Created by Tan on 25-Oct-23.
//

#include "wifi_esp32.h"

Wifi::Wifi() {
    currentApWifiSSID = nullptr;
    currentApWifiPassword = nullptr;
    currentStaWifiSSID = nullptr;
    currentStaWifiPassword = nullptr;
}

bool Wifi::init_ap_mode() {
    // Code to initialize the device in AP mode
    WiFi.mode(WIFI_AP);
    WiFi.softAP(currentApWifiSSID, currentApWifiPassword);
    return WiFi.softAPIP() != INADDR_NONE;
}

bool Wifi::init_sta_mode() const {
    // Code to initialize the device in STA (client) mode
    WiFi.setHostname(currentHostname);
    WiFi.mode(WIFI_STA);
    WiFi.begin(currentStaWifiSSID, currentStaWifiPassword);
    // We'll wait up to 10 seconds for a connection
    unsigned long startTime = millis();
    const unsigned long timeout = 10000; // 10 seconds timeout

    // Keep checking the status until we're connected or until the timeout
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime >= timeout) {
            Serial.println("[WiFi] Failed to connect within the timeout period.");
            return false;
        }

        delay(500); // Wait half a second before checking again

        // You can handle different cases here if you want to provide detailed feedback
        if (WiFi.status() == WL_NO_SSID_AVAIL) {
            Serial.println("[WiFi] SSID not found");
            return false;
        } else if (WiFi.status() == WL_CONNECT_FAILED) {
            Serial.println("[WiFi] Connection failed");
            return false;
        } else if (WiFi.status() == WL_CONNECTION_LOST) {
            Serial.println("[WiFi] Connection lost");
            return false;
        }
    }

    // If we get here, we are connected
    Serial.println("[WiFi] WiFi is connected!");
    Serial.print("[WiFi] IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

bool Wifi::init_mdns() {
    // Code to initialize mDNS service
    // if (MDNS.begin("esp8266")) {
    //     Serial.println("MDNS responder started");
    //     return true;
    // }
    // return false;
    return true; // Placeholder return value
}

void Wifi::init_web_page() {
    // Code to initialize web server or web page
    // You may need to start a web server and define route handlers
    // webServer.on("/", HTTP_GET, [this]() { handleRoot(); });
    // webServer.begin();
}

void Wifi::set_ap_wifi_credential(char *ssid, char *password) {
    currentApWifiSSID = ssid;
    currentApWifiPassword = password;
    char buffer[100];
    sprintf(buffer, "Set new AP Wifi credential to SSID: %s, password: %s", ssid, password);
    Serial.println(buffer);
}

void Wifi::set_sta_wifi_credential(char *ssid, char *password, char *hostname) {
    currentStaWifiSSID = ssid;
    currentStaWifiPassword = password;
    currentHostname = hostname;
    char buffer[100];
    sprintf(buffer, "Set new STA Wifi credential to SSID: %s, password: %s, hostname: %s", ssid, password, hostname);
    Serial.println(buffer);
}

void Wifi::terminate_ap_mode() {
    // Code to terminate AP mode
    WiFi.softAPdisconnect(true);
}

void Wifi::terminate_sta_mode() {
    // Code to terminate STA mode
    WiFi.disconnect(true);
}

void Wifi::terminate_web_page() {
    // Code to terminate the web server or web page
    // webServer.stop();
}