//
// Created by Tan on 25-Oct-23.
//

#include "rfid.h"

Rfid::Rfid() {

}

void Rfid::init(byte rx_pin, byte tx_pin) {
    Serial2.begin(115200, SERIAL_8N1, rx_pin, tx_pin);
    Serial.println(F("RFID module initiated"));
    set_tx_power(2600);
    get_hardware_version();
//    get_software_version();
}

String Rfid::hex2str(uint8_t num) {
    if (num > 0xf) {
        return String(num, HEX);
    } else {
        return ("0" + String(num, HEX));
    }
}

String Rfid::byte_array_to_hex_string(const uint8_t *byte_array, size_t array_length) {
    String hex_string = "";
    for (size_t i = 0; i < array_length; ++i) {
        if (byte_array[i] < 0x10) {
            hex_string += "0"; // Add a leading zero for values less than 0x10
        }
        hex_string += String(byte_array[i], HEX);
    }
    hex_string.toUpperCase(); // Convert the string to uppercase
    return hex_string;
}

// This method is specifically used for POLLING_MULTI, for other commands, use read_response
String Rfid::read_response_async() {
    String response = "";
    bool startDetected = false;
    unsigned long lastReadTime = millis();
    const unsigned long timeout = 1000; // Set a timeout period, e.g., 1000 milliseconds (1 second)
    const String noTagResponse = "BB01FF000115167E"; // "No tags found" message without spaces
    int noTagCount = 0; // Counter for "no tags found" messages

    while (true) {
        if (Serial2.available()) {
            byte readByte = Serial2.read();
            lastReadTime = millis(); // Update the last read time

            if (readByte == 0xBB) { // Check if the start byte is detected
                startDetected = true;
                response = "BB"; // Start building the response string
            } else if (startDetected) {
                char hexBuffer[3]; // 2 characters for the byte and 1 for the null terminator
                sprintf(hexBuffer, "%02X", readByte);
                response += hexBuffer;

                if (readByte == 0x7E) { // Check if the end byte is detected
                    if (response.equalsIgnoreCase(noTagResponse)) {
                        noTagCount++; // Increment "no tags found" counter
                        Serial.print("No tags found. Count: ");
                        Serial.println(noTagCount);

                        if (noTagCount >= 10) {
                            Serial.println("No tag threshold exceeded. Stopping scan.");
                            //Serial.println(F("Stop scanning multi RFID tags"));
                            //send_command((uint8_t *) STOP_POLLING_MULTI_CMD, sizeof(STOP_POLLING_MULTI_CMD));
                            break; // Exit the loop after 5 "no tags found" messages
                        }
                    } else {
                        noTagCount = 0; // Reset the counter if a tag is found
                        Serial.println(response); // Print the tag data
                    }
                    startDetected = false; // Reset the start detected flag for the next tag or message
                    response = ""; // Clear the response for the next message
                }
            }
        } else if (millis() - lastReadTime > timeout && !startDetected) {
            // If no more data is received for the duration of the timeout, break the loop
            Serial.println("Timeout: No more tags detected.");
            break;
        }
        delay(10); // Small delay to prevent reading too fast
    }

    Serial.print(F("Last response before stopping: "));
    // Returns the last response, or an empty string if no tags found
    return response;
}

String
Rfid::read_response(bool wait_for_success_confirmation, uint8_t *success_confirmation, size_t confirmation_size) {
    String success_confirmation_string = "";
    if (wait_for_success_confirmation) {
        success_confirmation_string = byte_array_to_hex_string(success_confirmation, confirmation_size);
//        Serial.print(F("Success confirmation string: "));
//        Serial.println(success_confirmation_string);
    }
    String response = "";
    bool startDetected = false;
    unsigned long lastReadTime = 0;
    const unsigned long timeout = 3000; // Set a timeout period, e.g., 1000 milliseconds (1 second)

    // Read the response until no more data is received for the duration of the timeout
    while (true) {
        if (Serial2.available()) {
            byte readByte = Serial2.read();
            lastReadTime = millis(); // Update the last read time

            if (readByte == 0xBB) { // Check if the start byte is detected
                startDetected = true;
                response = "";
                response += "BB"; // Include the start byte in the response
            } else if (startDetected) {
                char hexBuffer[4];
                sprintf(hexBuffer, "%02X", readByte);
                response += hexBuffer;

                if (readByte == 0x7E) { // Check if the end byte is detected
                    if (wait_for_success_confirmation) {
                        if (response.equalsIgnoreCase(success_confirmation_string)) {
                            Serial.print(F("Got success confirmation string: "));
                            Serial.println(success_confirmation_string);
                            break; // Exit the loop if success confirmation string found
                        }
                    }
                    Serial.println(response);
                    startDetected = false;
                }
            }
        } else if (millis() - lastReadTime > timeout && startDetected == false) {
            // If no more data is received for the duration of the timeout, break the loop
            break;
        }
        delay(10); // Small delay to prevent reading too fast
    }
    Serial.print(F("Last response from RFID module: "));
    return response; // Return the last response, if needed
}

/*! @brief Send command.*/
void Rfid::send_command(uint8_t *data, size_t size) {
    Serial.println(F("Sending command"));
    Serial2.write(data, size);
    //Serial2.flush(); // Wait for the data to be completely sent
}

/*! @brief Get hardware version information.*/
String Rfid::get_hardware_version() {
    Serial.println(F("Getting RFID hardware version"));
    Serial.println(F("Hardware version: "));
    send_command((uint8_t *) HARDWARE_VERSION_CMD, sizeof(HARDWARE_VERSION_CMD));
    Serial.println(read_response(false, nullptr, 8));
    return "";
}

/*! @brief Get software version information.*/
String Rfid::get_software_version() {
    Serial.println(F("Getting RFID software version"));
    send_command((uint8_t *) SOFTWARE_VERSION_CMD, sizeof(SOFTWARE_VERSION_CMD));
    Serial.println(read_response(false, nullptr, 8));
    return "";
}

/*! @brief Save the card information.*/
void Rfid::print_rfid_tag_info() {
    String rssi = hex2str(buffer[5]);
    String pc = hex2str(buffer[6]) + hex2str(buffer[7]);
    String epc = "";

    for (uint8_t i = 8; i < 20; i++) {
        epc += hex2str(buffer[i]);
    }

    Serial.println("pc: " + pc);
    Serial.println("rssi: " + rssi);
    Serial.println("epc: " + epc);
    for (uint8_t i = 0; i < 24; i++) {
        Serial.print(hex2str(buffer[i]));
    }
    Serial.println(" ");
}

void Rfid::polling_once() {
    send_command((uint8_t *) POLLING_ONCE_CMD, sizeof(POLLING_ONCE_CMD));
    Serial.println(read_response_async());
//    while (wait_msg()) {
//        if (buffer[23] == 0x7e) {
//            print_rfid_tag_info();
//        }
//    }
}

void Rfid::polling_multi() {
//    send_command((uint8_t *) STOP_POLLING_MULTI_CMD, sizeof(STOP_POLLING_MULTI_CMD),
//                 true, (uint8_t *) SUCCESSFULLY_STOP_POLLING_MULTI);
    send_command((uint8_t *) POLLING_MULTIPLE_CMD, sizeof(POLLING_MULTIPLE_CMD));
    Serial.println(read_response_async());
}

void Rfid::scan_rfid_tag() const {
    switch (scanning_mode) {
        case SINGLE_SCAN:
            polling_once();
            break;
        case MULTI_SCAN:
            Serial.println(F("Start scanning multi RFID tags"));
            polling_multi();
            Serial.println(F("Stop scanning multi RFID tags"));
            send_command((uint8_t *) STOP_POLLING_MULTI_CMD, sizeof(STOP_POLLING_MULTI_CMD));
            Serial.println(read_response(true, (uint8_t *) SUCCESSFULLY_STOP_POLLING_MULTI, 8));
            break;
    }
}

rfid_scan_result Rfid::get_rfid_scan_result() {
    rfid_scan_result _rfid_scan_result;
    return _rfid_scan_result;
}

void Rfid::set_scanning_mode(rfid_scanning_mode_t _scanning_mode) {
    scanning_mode = _scanning_mode;
}

void Rfid::set_tx_power(uint16_t db) {
    Serial.println(F("Start setting tx power"));
    send_command((uint8_t *) SET_TX_POWER, sizeof(SET_TX_POWER));
    Serial.print(F("Set TX power received: "));
    Serial.println(read_response(true, (uint8_t *) SUCCESSFULLY_SET_TX_POWER, 8));
}


