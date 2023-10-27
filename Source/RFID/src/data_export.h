//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_DATA_EXPORT_H
#define RFID_DATA_EXPORT_H

#include "Arduino.h"

typedef struct row {
    int timestamp;
} row;

typedef struct collection {
    row _row;
} collection;

extern row data_row;
extern collection data_collection;

class DataExport {
public:
    DataExport();

    void insert_row(row _row);

    void update_row(int timestamp);

    void delete_row(int timestamp);
};
#endif //RFID_DATA_EXPORT_H
