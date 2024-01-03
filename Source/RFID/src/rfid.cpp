//
// Created by Tan on 25-Oct-23.
//

#include "rfid.h"

Rfid::Rfid() {

}

bool Rfid::init() {
    return true;
}

/*! @brief Waiting for a period of time to receive a message
    @return True if the message is available at the specified time and in the
   specified range, otherwise false..*/
bool Rfid::wait_msg(unsigned long time) {
//    unsigned long start = millis();
//    uint8_t i           = 0;
//    cleanBuffer();
//    while (_serial->available() || (millis() - start) < time) {
//        if (_serial->available()) {
//            uint8_t b = _serial->read();
//            buffer[i] = b;
//            i++;
//            if (b == 0x7e) {
//                break;
//            }
//        }
//    }
//    if (buffer[0] == 0xbb && buffer[i - 1] == 0x7e) {
//        return true;
//    } else {
//        return false;
//    }
}

/*! @brief Send command.*/
void Rfid::send_command(uint8_t *data, size_t size) {
//    _serial->write(data, size);
}

void Rfid::scan_rfid_tag() {

}

rfid_scan_result Rfid::get_rfid_scan_result() {
    rfid_scan_result _rfid_scan_result;
    return _rfid_scan_result;
}