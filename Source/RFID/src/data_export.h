//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_DATA_EXPORT_H
#define RFID_DATA_EXPORT_H

#include "Arduino.h"

typedef struct data_row {
    int timestamp;
} data_row;

typedef struct data_collection {
    data_row _row;
} data_collection;

extern data_row dataRow;
extern data_collection dataCollection;

class DataExport {
public:
    DataExport();

    void insert_row(data_row _row);

    void update_row(int timestamp);

    void delete_row(int timestamp);
};
#endif //RFID_DATA_EXPORT_H
