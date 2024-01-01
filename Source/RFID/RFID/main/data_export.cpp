//
// Created by Tan on 25-Oct-23.
//

#include "Arduino.h"
#include "data_export.h"

DataExport::DataExport() {
    //Serial.begin(115200);
    //while (!Serial)  // Wait for the serial connection to be establised.
    //    delay(50);
    //Serial.println(F(""));
    //Serial.println(F("Data Export Initialzed"));
}

void DataExport::insert_row(data_row _row) {

}

void DataExport::update_row(int timestamp) {

}

void DataExport::delete_row(int timestamp) {

}
