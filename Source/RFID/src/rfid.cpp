//
// Created by Tan on 25-Oct-23.
//

#include "rfid.h"

Rfid::Rfid() {
    scanned_tag_count = 0;
}

void Rfid::init(byte rx_pin, byte tx_pin) {
    Serial2.begin(115200, SERIAL_8N1, rx_pin, tx_pin);
    while (!Serial2.available()) {
        Serial.println(F("Waiting for RFID module starting"));
    }
    Serial.println(F("RFID module initiated"));
    get_hardware_version();
    set_tx_power(2600);
//    get_software_version();
}

void Rfid::clean_buffer() {
    // Clean buffer
    for (int i = 0; i < 200; i++) {
        buffer[i] = 0;
    }
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
void Rfid::read_multi_scan_response() {
    String response = "";
    bool startDetected = false;
    unsigned long lastReadTime = millis();
    const unsigned long timeout = 1000; // Set a timeout period
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

                        if (noTagCount >= 5) {
                            Serial.println("No tag threshold exceeded. Stopping scan.");
                            //Serial.println(F("Stop scanning multi RFID tags"));
                            //send_command((uint8_t *) STOP_POLLING_MULTI_CMD, sizeof(STOP_POLLING_MULTI_CMD));
                            break; // Exit the loop after 10 "no tags found" messages
                        }
                    } else {
                        noTagCount = 0; // Reset the counter if a tag is found
                        // If the response type is EPC_READING, process the EPC
                        if (is_valid_epc_response(response)) {
                            Serial.println("Valid EPC response. Start appending");
                            // Parse the EPC from the response
                            Serial.println("Raw response: " + response);
                            String epc = response.substring(16, 40);

                            // Check for duplicatesW
                            if (!is_duplicate_scan(epc)) {
                                // If not a duplicate, add to scan_results and increment scanned_tag_count
                                scan_results[scanned_tag_count].epc = epc;
                                scanned_tag_count++;

                                // Optionally print the new EPC
                                Serial.print(F("New EPC scanned: "));
                                Serial.println(epc);
                            } else {
                                Serial.print(F("Duplicate EPC: "));
                                Serial.println(epc);
                            }
                        } else {
                            Serial.println("Invalid EPC response or no tag read.");
                        }
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
    //return response;
}

void Rfid::read_single_scan_response() {
    clean_buffer();

    uint8_t buffer_index = 0;

    static bool is_end_byte_detected = false;
    static bool is_tag_not_found = false;

    // Read data from RFID module when this method is called
    while (Serial2.available()) {
        if (Serial2.available()) {
            byte readByte = Serial2.read();
            buffer[buffer_index] = readByte;
            ++buffer_index;
            // Check if the end byte is detected
            if (readByte == 0x7e) {
                is_end_byte_detected = true;

                // Check if we have a valid response frame, 0xbb...........0x7e
                if (buffer[0] == 0xbb && buffer[buffer_index - 1] == 0x7e) {
                    // Check if response is tag not found
                    if (buffer_index == sizeof(RFID_TAG_NOT_FOUND) &&
                        memcmp(buffer, RFID_TAG_NOT_FOUND, sizeof(RFID_TAG_NOT_FOUND)) == 0) {
                        is_tag_not_found = true;
                        break;
                    } else {
                        // We have a new tag, print its EPC
                        print_epc(buffer, buffer_index);
                    }
                }
            }
        }
    }

    // Reset bool
    is_end_byte_detected = false;
    is_tag_not_found = false;
}

bool Rfid::read_response(unsigned long timeout) {
    unsigned long start_time = millis();

    uint8_t buffer_index = 0;

    clean_buffer();

    // Read the response until no more data is received for the duration of the timeout
    while (Serial2.available() || (millis() - start_time < timeout)) {
        if (Serial2.available()) {
            byte readByte = Serial2.read();
            buffer[buffer_index] = readByte;
            ++buffer_index;
            // Check if the end byte is detected
            if (readByte == 0x7e) {
                break;
            }
        }
    }

    if (buffer[0] == 0xbb && buffer[buffer_index - 1] == 0x7e) {
        return true;
    } else {
        return false;
    }
}

void Rfid::send_command(uint8_t *data, size_t size) {
    Serial.println(F("Sending command"));
    Serial2.write(data, size);
    //Serial2.flush(); // Wait for the data to be completely sent
}

void Rfid::get_hardware_version() {
    Serial.println(F("Getting RFID hardware version: "));
    send_command((uint8_t *) HARDWARE_VERSION_CMD, sizeof(HARDWARE_VERSION_CMD));
    if (read_response(3000)) {
        String info;
        for (uint8_t i = 0; i < 50; i++) {
            info += (char) buffer[6 + i];
            if (buffer[8 + i] == 0x7e) {
                break;
            }
        }
        Serial.println(info);
    } else {
        Serial1.println(F("Error in getting hardware version"));
    }
}

void Rfid::get_software_version() {
    Serial.println(F("Getting RFID software version"));
    send_command((uint8_t *) SOFTWARE_VERSION_CMD, sizeof(SOFTWARE_VERSION_CMD));
    read_response(3000);
}

void Rfid::polling_once() {
    send_command((uint8_t *) POLLING_ONCE_CMD, sizeof(POLLING_ONCE_CMD));
    read_single_scan_response();
    //Serial.println(read_response(false, nullptr, 8, 20, EPC_READING));
}

void Rfid::polling_multi() {
//    send_command((uint8_t *) STOP_POLLING_MULTI_CMD, sizeof(STOP_POLLING_MULTI_CMD),
//                 true, (uint8_t *) SUCCESSFULLY_STOP_POLLING_MULTI);
    send_command((uint8_t *) POLLING_MULTIPLE_CMD, sizeof(POLLING_MULTIPLE_CMD));
    read_multi_scan_response();
}

void Rfid::scan_rfid_tag() {
    switch (scanning_mode) {
        case SINGLE_SCAN:
            Serial.println(F("Start scanning RFID tags once"));
            polling_once();
            Serial.print(F("Total scanned RFID tags currently: "));
            Serial.println(scanned_tag_count);
            stop_scanning();
            //for (int i = 0; i < 200; ++i) {
            //Serial.print(scan_results[i].epc);
            //Serial.print(", ");
            //}
            //Serial.println();
            break;
        case MULTI_SCAN:
            Serial.println(F("Start scanning multi RFID tags"));
            polling_multi();
            stop_scanning();
            break;
    }
}

void Rfid::set_scanning_mode(rfid_scanning_mode_t _scanning_mode) {
    scanning_mode = _scanning_mode;
}

void Rfid::set_tx_power(uint16_t db) {
    Serial.println(F("Start setting tx power"));
    send_command((uint8_t *) SET_TX_POWER, sizeof(SET_TX_POWER));
    if (read_response(500)) {
        Serial.println(F("Set TX power successfully"));
    } else {
        Serial.println(F("Set TX power failed"));
    }
//    Serial.println(F("Start setting high sensitivity"));
//    send_command((uint8_t *) SET_HIGH_SENSITIVITY, sizeof(SET_HIGH_SENSITIVITY));
}

bool Rfid::is_duplicate_scan(const String &epc) {
    for (int i = 0; i < scanned_tag_count; ++i) {
        if (scan_results[i].epc.equalsIgnoreCase(epc)) {
            return true;
        }
    }
    return false;
}

// Method to validate the EPC response
bool Rfid::is_valid_epc_response(const String &response) {
    // Convert the response to uppercase for case-insensitive comparison
    String upperResponse = response;
    upperResponse.toUpperCase();

    // Define the expected start and end markers of the EPC response
    const String expectedStart = "BB0222";
    const String expectedLength = "0011"; // Indicating 17 bytes follow
    const String expectedEnd = "7E";

    // Check if the response starts with the expected start sequence
    if (!upperResponse.startsWith(expectedStart)) {
        return false;
    }

    // Check if the response ends with the expected end sequence
    if (!upperResponse.endsWith(expectedEnd)) {
        return false;
    }

    // Check if the length field in the response matches the expected length
//    int lengthIndex = expectedStart.length(); // The length field starts right after the expectedStart
//    String lengthField = upperResponse.substring(lengthIndex, lengthIndex + 4);
//    if (lengthField != expectedLength) {
//        return false;
//    }

    // Calculate the expected response length: start (6) + length (4) + data (36) + end (2)
    int expectedResponseLength = 48;
    if (upperResponse.length() != expectedResponseLength) {
        return false;
    }

    // If all checks pass, return true
    return true;
}

void Rfid::stop_scanning() {
    Serial.println(F("Stop scanning multi RFID tags"));
    send_command((uint8_t *) STOP_POLLING_MULTI_CMD, sizeof(STOP_POLLING_MULTI_CMD));
    read_response(500);
    //Serial.println(read_response(true, (uint8_t *) SUCCESSFULLY_STOP_POLLING_MULTI, 8, 3000, NORMAL_READING));
}

void Rfid::print_epc(const uint8_t *buffer, uint8_t buffer_size) {
    // Check if the buffer size is sufficient
    if (buffer_size >= 18) {
        Serial.print("EPC: 0x");
        for (uint8_t i = 8; i < 18; i++) {
            if (buffer[i] < 0x10) {
                Serial.print("0");
            }
            Serial.print(buffer[i], HEX);
        }
        Serial.println();
    } else {
        Serial.println("Invalid EPC data");
    }
}