//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_FS_H
#define RFID_FS_H

#include "Arduino.h"

class FS {
public:
    bool isFsLoaded;

    FS();

    bool load_fs();

    bool write_to_file();
};
#endif //RFID_FS_H
