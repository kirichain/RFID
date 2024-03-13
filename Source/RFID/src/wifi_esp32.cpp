//
// Created by Tan on 25-Oct-23.
//

#include "wifi_esp32.h"

AsyncWebServer async_server(80);

Wifi::Wifi() {
    currentApWifiSSID = nullptr;
    currentApWifiPassword = nullptr;
    currentStaWifiSSID = nullptr;
    currentStaWifiPassword = nullptr;
}

void Wifi::init_ap_mode() {
    // Code to initialize the device in AP mode
    WiFi.mode(WIFI_AP);
    if (WiFi.softAP(currentApWifiSSID, currentApWifiPassword)) {
        Serial.println(F("Init AP Wi-Fi successfully"));
        Serial.print(F("Wi-Fi AP IP is: "));
        Serial.println(WiFi.softAPIP());
        wait_for_new_wifi_setting();
    } else {
        Serial.println(F("Init AP Wi-Fi failed"));
    }
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

void Wifi::set_ap_wifi_credential(char *ssid, char *password) {
    currentApWifiSSID = ssid;
    currentApWifiPassword = password;
    char buffer[100];
    sprintf(buffer, "Set new AP Wifi credential to ssid: %s, password: %s", ssid, password);
    Serial.println(buffer);
}

void Wifi::set_sta_wifi_credential(char *ssid, char *password, char *hostname) {
    currentStaWifiSSID = ssid;
    currentStaWifiPassword = password;
    currentHostname = hostname;
    char buffer[100];
    sprintf(buffer, "Set new STA Wifi credential to ssid: %s, password: %s, hostname: %s", ssid, password, hostname);
    Serial.println(buffer);
}

void Wifi::terminate_ap_mode() {
    // Code to terminate AP mode
    Serial.println(F("Terminate AP Wi-Fi"));
    WiFi.softAPdisconnect(true);
}

void Wifi::terminate_sta_mode() {
    // Code to terminate STA mode
    WiFi.disconnect(true);
    is_sta_mode_enabled = false;
}

int Wifi::scan_wifi_networks() {
    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    // WiFi.scanNetworks will return the number of networks found.
    wifi_networks_count = WiFi.scanNetworks();
    Serial.println("Scan Wi-Fi networks done");
    if (wifi_networks_count == 0) {
        Serial.println("No Wi-Fi networks found");
    }
    if (wifi_networks_count == WIFI_SCAN_FAILED) {
        Serial.println("Wi-Fi scan failed");
        // Handle the scan failure (e.g., by returning an error code)
        return -1;
    } else if (wifi_networks_count == WIFI_SCAN_RUNNING) {
        Serial.println("Wi-Fi scan is still running");
        // Handle the ongoing scan appropriately
        return -1;
    } else if (wifi_networks_count == WL_SCAN_COMPLETED) {
        Serial.println("Wi-Fi scan completed");
        // Normal operation would continue from here
    } else {
        Serial.print(F("Found "));
        Serial.print(wifi_networks_count);
        Serial.println(F(" available Wi-Fi networks"));

        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < wifi_networks_count && i < 10; ++i) {
            // Store the SSID in the wifi_networks array
            strncpy(wifi_networks[i].ssid, WiFi.SSID(i).c_str(), sizeof(wifi_networks[i].ssid) - 1);
            wifi_networks[i].ssid[sizeof(wifi_networks[i].ssid) - 1] = '\0'; // Ensure null-termination
            // Store RSSI in the wifi_networks array
            wifi_networks[i].rssi = WiFi.RSSI(i);

            // Print SSID and RSSI for each network found
            Serial.printf("%2d", i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4d", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2d", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i)) {
                case WIFI_AUTH_OPEN:
                    Serial.print("open");
                    break;
                case WIFI_AUTH_WEP:
                    Serial.print("WEP");
                    break;
                case WIFI_AUTH_WPA_PSK:
                    Serial.print("WPA");
                    break;
                case WIFI_AUTH_WPA2_PSK:
                    Serial.print("WPA2");
                    break;
                case WIFI_AUTH_WPA_WPA2_PSK:
                    Serial.print("WPA+WPA2");
                    break;
                case WIFI_AUTH_WPA2_ENTERPRISE:
                    Serial.print("WPA2-EAP");
                    break;
                case WIFI_AUTH_WPA3_PSK:
                    Serial.print("WPA3");
                    break;
                case WIFI_AUTH_WPA2_WPA3_PSK:
                    Serial.print("WPA2+WPA3");
                    break;
                case WIFI_AUTH_WAPI_PSK:
                    Serial.print("WAPI");
                    break;
                default:
                    Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    return wifi_networks_count;
}

String Wifi::get_mac_addr() {
    Serial.print(F("Mac address: "));
    Serial.println(WiFi.macAddress());
    mac_address = WiFi.macAddress();
    return WiFi.macAddress();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void Wifi::handleSetWiFiConnection(AsyncWebServerRequest *request) {
    if (request->hasParam("ssid") && request->hasParam("password")) {
        is_sta_wifi_reconnected = true;
        is_sta_mode_enabled = true;

        String ssid = request->getParam("ssid")->value();
        String password = request->getParam("password")->value();

        // Copy the credentials into the class member variables
        ssid.toCharArray(ssidArray, sizeof(ssidArray));
        password.toCharArray(passwordArray, sizeof(passwordArray));

        // Call the function to set the STA Wi-Fi credentials
        set_sta_wifi_credential(ssidArray, passwordArray, currentHostname);

        // Terminate AP mode and clean up to save resources
        terminate_ap_mode();
        async_server.end();

//        // Initialize STA mode with the new credentials
//        if (init_sta_mode()) {
//            request->send(200, "text/plain", "STA mode initialized with SSID = " + ssid + " & Password = " + password);
//
//        } else {
//            request->send(500, "text/plain", "Failed to initialize STA mode");
//        }
    } else {
        // In case the SSID or Password was not provided
        request->send(400, "text/plain", "SSID and Password parameters are required");
    }
}

void Wifi::wait_for_new_wifi_setting() {
    async_server.onNotFound(notFound);

    // Send a GET request to <IP>/?ssid=<ssid>&password=<password>
    // http://192.168.4.1/set-wifi-connection?ssid=ERPLTD&password=erp@@2020
    // http://192.168.4.1/set-wifi-connection?ssid=kiri&password=101conchodom
    async_server.on("/set-wifi-connection", HTTP_GET, [this](AsyncWebServerRequest *request) {
        handleSetWiFiConnection(request);
    });

    async_server.begin();
}