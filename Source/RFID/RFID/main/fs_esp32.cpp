//
// Created by lenovo on 3/19/2024.
//

#include "fs_esp32.h"

FS32::FS32() {

}

bool FS32::save_settings(const String& ssid, const String& password, const String& mes_package, const String& mes_package_group) {
    File file = SPIFFS.open("/device_data.txt", "w");

    if (!file) {
        Serial.println(F("Failed to open device_data.txt for writing"));
        return false;
    }

    // Create the JSON document
    JsonDocument doc;

    // Set the values in the document
    doc["ssid"] = ssid;
    doc["password"] = password;
    doc["mes_package"] = mes_package;
    doc["mes_package_group"] = mes_package_group;

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
        // Print values for debugging
        Serial.print(F("SSID: "));
        Serial.println(ssid);
        Serial.print(F("Password: "));
        Serial.println(password);
        Serial.print(F("MES Package: "));
        Serial.println(mes_package);
        Serial.print(F("MES Package Group: "));
        Serial.println(mes_package_group);
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
