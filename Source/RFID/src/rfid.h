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

// Hardware version
const uint8_t HARDWARE_VERSION_CMD[] = {0xBB, 0x00, 0x03, 0x00,
                                        0x01, 0x00, 0x04, 0x7E};
// Software version
const uint8_t SOFTWARE_VERSION_CMD[] = {0xBB, 0x00, 0x03, 0x00,
                                        0x01, 0x01, 0x05, 0x7E};
// Single polling instruction
const uint8_t POLLING_ONCE_CMD[] = {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E};
// Multiple polling instructions
const uint8_t POLLING_MULTIPLE_CMD[] = {0xBB, 0x00, 0x27, 0x00, 0x03,
                                        0x22, 0x27, 0x10, 0x83, 0x7E};
// Set the SELECT mode
const uint8_t SET_SELECT_MODE_CMD[] = {0xBB, 0x00, 0x12, 0x00,
                                       0x01, 0x01, 0x14, 0x7E};
// Set the SELECT parameter instruction
const uint8_t SET_SELECT_PARAMETER_CMD[] = {
        0xBB, 0x00, 0x0C, 0x00, 0x13, 0x01, 0x00, 0x00, 0x00,
        0x20, 0x60, 0x00, 0x30, 0x75, 0x1F, 0xEB, 0x70, 0x5C,
        0x59, 0x04, 0xE3, 0xD5, 0x0D, 0x70, 0xAD, 0x7E};
// SELECT OK RESPONSE
const uint8_t SET_SELECT_OK[] = {0xBB, 0x01, 0x0C, 0x00,
                                 0x01, 0x00, 0x0E, 0x7E};
const uint8_t GET_SELECT_PARAMETER_CMD[] = {0xBB, 0x00, 0x0B, 0x00,
                                            0x00, 0x0B, 0x7E};
const uint8_t READ_STORAGE_CMD[] = {0xBB, 0x00, 0x39, 0x00, 0x09, 0x00,
                                    0x00, 0xFF, 0xFF, 0x03, 0x00, 0x00,
                                    0x00, 0x02, 0x45, 0x7E};
const uint8_t READ_STORAGE_ERROR[] = {0xBB, 0x01, 0xFF, 0x00,
                                      0x01, 0x09, 0x0A, 0x7E};
const uint8_t WRITE_STORAGE_CMD[] = {0xBB, 0x00, 0x49, 0x00, 0x0D, 0x00, 0x00,
                                     0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x02,
                                     0x12, 0x34, 0x56, 0x78, 0x6D, 0x7E};
const uint8_t WRITE_STORAGE_ERROR[] = {0xBB, 0x01, 0xFF, 0x00,
                                       0x01, 0x10, 0x0A, 0x7E};
// Set the transmitting power
const uint8_t SET_TX_POWER[] = {0xBB, 0x00, 0xB6, 0x00, 0x02,
                                0x07, 0xD0, 0x8F, 0x7E};

class Rfid {
public:
    rfid_scanning_mode_t scanning_mode;
    uint8_t buffer[200] = {0};

    Rfid();

    void init();

    static String hex2str(uint8_t num);

    void clean_buffer();

    bool wait_msg();

    static void send_command(uint8_t *data, size_t size);

    String get_hardware_version();

    String get_software_version();

    void polling_once();

    void print_rfid_tag_info();

    void scan_rfid_tag();

    void set_scanning_mode(rfid_scanning_mode_t _scanning_mode);

    bool set_tx_power(uint16_t db);

    rfid_scan_result get_rfid_scan_result();
};

#endif //RFID_RFID_H
