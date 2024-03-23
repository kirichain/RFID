//
// Created by lenovo on 3/19/2024.
//

#include "fs_esp32.h"

FS32::FS32() {

}

bool FS32::save_settings(const String& _ssid, const String& _password, const String& _mes_package, const String& _mes_package_group, const String& _is_muted) {
    File file = SPIFFS.open("/device_data.txt", "w");

    if (!file) {
        Serial.println(F("Failed to open device_data.txt for writing"));
        return false;
    }

    // Create the JSON document
    JsonDocument doc;

    // Set the values in the document
    doc["ssid"] = _ssid;
    doc["password"] = _password;
    doc["mes_package"] = _mes_package;
    doc["mes_package_group"] = _mes_package_group;
    doc["is_muted"] = _is_muted;

    // Update data
    ssid = _ssid;
    password = _password;
    mes_package = _mes_package;
    mes_package_group = _mes_package_group;
    is_muted = _is_muted;

    Serial.println(F("JSON document created with the following settings:"));
    serializeJsonPretty(doc, Serial); // Print the JSON document in a pretty way to Serial for debugging

    // Serialize JSON to file
    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
        file.close();
        return false;
    }

    // Data written successfully
    Serial.println(F("Settings saved"));
    file.close();
    return true;
}

bool FS32::read_saved_settings() {
    File file = SPIFFS.open("/device_data.txt", "r+");

    ssid = "";
    password = "";
    mes_package = "";
    mes_package_group = "";
    is_muted = "";

    if (!file) {
        Serial.println(F("Device setting file open failed---------"));
        return false;
    }

    Serial.println(F("Device setting file opened--------"));
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, file);
    if (err) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
        file.close();
    } else {
        Serial.println("Deserialize done. Start getting settings now");
        // Assign read settings to strings accordingly
        ssid = doc["ssid"].as<String>();
        password = doc["password"].as<String>();
        mes_package = doc["mes_package"].as<String>();
        mes_package_group = doc["mes_package_group"].as<String>();
        is_muted = doc["is_muted"].as<String>();
        // Print values for debugging
        Serial.print(F("SSID: "));
        Serial.println(ssid);
        Serial.print(F("Password: "));
        Serial.println(password);
        Serial.print(F("MES Package: "));
        Serial.println(mes_package);
        Serial.print(F("MES Package Group: "));
        Serial.println(mes_package_group);
        Serial.print(F("Is muted: "));
        Serial.println(is_muted);
    }
    file.close();
    return true;
}

bool FS32::init_spiffs() {
    if (!SPIFFS.begin()) {
        Serial.println(F("An Error has occurred while mounting SPIFFS-----------"));
        return false;
    }
    Serial.println(F("Mounting SPIFFS done-----------"));
    return true;
}
