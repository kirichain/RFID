//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_DATA_EXPORT_H
#define RFID_DATA_EXPORT_H

#include "Arduino.h"
#include "structs.h"


class DataExport {
public:
    data_row dataRow;
    data_collection dataCollection;

    DataExport();

    void insert_row(data_row _row);

    void update_row(int timestamp);

    void delete_row(int timestamp);
};

#endif //RFID_DATA_EXPORT_H
