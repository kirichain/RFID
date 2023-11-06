//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_RFID_H
#define RFID_RFID_H

#include "Arduino.h"

typedef struct rfid_scan_result {
    int timestamp;
    int id;

} rfid_scan_result;

extern rfid_scan_result scan_result;

class Rfid {
public:
    Rfid();

    bool init();

    bool scan_rfid_tag();

    rfid_scan_result get_rfid_scan_result();
};
#endif //RFID_RFID_H
