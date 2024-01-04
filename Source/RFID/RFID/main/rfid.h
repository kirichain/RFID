//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_RFID_H
#define RFID_RFID_H

#include "Arduino.h"
#include "structs.h"

/*
Header Type Command PL(MSB) PL(LSB) Parameter Checksum End
  BB    00     07     00      01        01       09     7E
                     param length
*/

class Rfid {
public:
    Rfid();

    bool init();

    bool wait_msg(unsigned long time);

    void send_command(uint8_t *data, size_t size);

    void scan_rfid_tag();

    rfid_scan_result get_rfid_scan_result();
};

#endif //RFID_RFID_H
