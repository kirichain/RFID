//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_RFID_H
#define RFID_RFID_H

#include "Arduino.h"

/*

Header Type Command PL(MSB) PL(LSB) Parameter Checksum End
  BB    00     07     00      01        01       09     7E
                     param length

*/


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
