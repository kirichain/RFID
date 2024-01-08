//
// Created by Tan on 25-Oct-23.
//

#include "rfid.h"

Rfid::Rfid() {

}

/*! Start connecting to rfid reader module using hardware serial _serial on TX-RX pins 16-17 */
void Rfid::init() {
    Serial1.begin(115200, SERIAL_8N1, 16, 17);
    set_tx_power(2600);
    get_hardware_version();
    get_software_version();
}

String Rfid::hex2str(uint8_t num) {
    if (num > 0xf) {
        return String(num, HEX);
    } else {
        return ("0" + String(num, HEX));
    }
}

/*! @brief Clear the buffer.*/
void Rfid::clean_buffer() {
    for (byte i = 0; i < 200; i++) {
        buffer[i] = 0;
    }
}

/*! @brief Waiting for a period of time to receive a message
    @return True if the message is available at the specified time and in the
   specified range, otherwise false..*/
bool Rfid::wait_msg() {
    uint8_t i = 0;
    clean_buffer();
    while (Serial1.available()) {
        if (Serial1.available()) {
            uint8_t b = Serial1.read();
            buffer[i] = b;
            i++;
            if (b == 0x7e) {
                break;
            }
        }
    }
    if (buffer[0] == 0xbb && buffer[i - 1] == 0x7e) {
        return true;
    } else {
        return false;
    }
}

/*! @brief Send command.*/
void Rfid::send_command(uint8_t *data, size_t size) {
    Serial1.write(data, size);
}

/*! @brief Get hardware version information.*/
String Rfid::get_hardware_version() {
    send_command((uint8_t *) HARDWARE_VERSION_CMD, sizeof(HARDWARE_VERSION_CMD));
    if (wait_msg()) {
        String info;
        for (uint8_t i = 0; i < 50; i++) {
            info += (char) buffer[6 + i];
            if (buffer[8 + i] == 0x7e) {
                break;
            }
        }
        return info;
    } else {
        return "ERROR IN GET HARDWARE VERSION";
    }
}

/*! @brief Get software version information.*/
String Rfid::get_software_version() {
    send_command((uint8_t *) SOFTWARE_VERSION_CMD, sizeof(SOFTWARE_VERSION_CMD));
    if (wait_msg()) {
        String info;
        for (uint8_t i = 0; i < 50; i++) {
            info += (char) buffer[6 + i];
            if (buffer[8 + i] == 0x7e) {
                break;
            }
        }
        return info;
    } else {
        return "ERROR IN GET SOFTWARE VERSION";
    }
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
    while (wait_msg()) {
        if (buffer[23] == 0x7e) {
            print_rfid_tag_info();
        }
    }
}

void Rfid::scan_rfid_tag() {
    switch (scanning_mode) {
        case SINGLE_SCAN:
            polling_once();
            break;
        case MULTI_SCAN:
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

bool Rfid::set_tx_power(uint16_t db) {
    memcpy(buffer, SET_TX_POWER, sizeof(SET_TX_POWER));
    buffer[5] = (db >> 8) & 0xff;
    buffer[6] = db & 0xff;

    uint8_t check = 0;

    for (uint8_t i = 1; i < 7; i++) {
        check += buffer[i];
    }
    buffer[7] = check & 0xff;
    send_command(buffer, sizeof(SET_TX_POWER));
    if (wait_msg()) {
        if (buffer[2] != 0xB6) {
            return false;
        }
        return true;
    } else {
        return false;
    }
}
