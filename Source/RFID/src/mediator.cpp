//
// Created by Tan on 25-Oct-23.
//

#include "Arduino.h"
#include "mediator.h"
#include "battery.h"
#include "buzzer.h"
#include "config.h"
#include "conveyor.h"
#include "data_export.h"
#include "data_import.h"
#include "display.h"
#include "fs.h"
#include "iam.h"
#include "json.h"
#include "mesh_network.h"
#include "modbus_rs485.h"
#include "operation.h"
#include "ota.h"
#include "package.h"
#include "peripherals.h"
#include "qr_code.h"
#include "qr_code_scanner.h"
#include "request.h"
#include "rfid.h"
#include "sd_card.h"
#include "sensor.h"
#include "socket.h"
#include "user.h"
#include "warehouse.h"
#include "web_page.h"
#include "wifi.h"
#include "ws2812b.h"
#include "lvgl.h"

Ws2812b ws2812b;
Wifi wifi;
DataExport dataExport;
DataImport dataImport;
Request request;
Conveyor conveyor;

Mediator::Mediator() {
    data_row.timestamp = NULL;
    taskArgs.function = "";
}

void Mediator::get() {

}

void Mediator::set() {

}

void Mediator::execute_task(task task) {
    switch (task) {
        case CHECK_FS:
            Serial.println(F("Execute task CHECK_FS"));
            break;
        case LOAD_FS:
            Serial.println(F("Execute task LOAD_FS"));
            break;
        case CHECK_CONNECTION:
            Serial.println(F("Execute task CHECK_CONNECTION"));
            break;
        case INIT_AP_WIFI:
            Serial.println(F("Execute task INIT_AP_WIFI"));
            break;
        case INIT_STA_WIFI:
            Serial.println(F("Execute task INIT_STA_WIFI"));
            break;
        case GET_OPERATING_MODE:
            Serial.println(F("Execute task GET_OPERATING_MODE"));
            break;
        case SET_OPERATING_MODE:
            Serial.println(F("Execute task SET_OPERATING_MODE"));
            break;
        case RENDER_OPERATING_MODE:
            Serial.println(F("Execute task RENDER_OPERATING_MODE"));
            break;
        case RENDER_FEATURE:
            Serial.println(F("Execute task RENDER_FEATURE"));
            break;
        case GET_FEATURE:
            Serial.println(F("Execute task GET_FEATURE"));
            break;
        case GET_NAVIGATION:
            Serial.println(F("Execute task GET_NAVIGATION"));
            break;
        case SET_FEATURE:
            Serial.println(F("Execute task SET_FEATURE"));
            break;
        case SET_FEATURE_TASK:
            Serial.println(F("Execute task SET_FEATURE_TASK"));
            break;
        case SET_FEATURE_TASK_STATUS:
            Serial.println(F("Execute task SET_FEATURE_TASK_STATUS"));
            break;
        case GET_FEATURE_TASK_STATUS:
            Serial.println(F("Execute task GET_FEATURE_TASK_STATUS"));
            break;
        case GET_RFID_SCAN_DATA:
            Serial.println(F("Execute task GET_RFID_SCAN_DATA"));
            break;
        case EXPORT_DATA_TO_SD_CARD:
            Serial.println(F("Execute task EXPORT_DATA_TO_SD_CARD"));
            break;
        case EXPORT_DATA_TO_SERVER:
            Serial.println(F("Execute task EXPORT_DATA_TO_SERVER"));
            break;
        case SCAN_RFID_TAG:
            Serial.println(F("Execute task SCAN_RFID_TAG"));
            break;
        case INSERT_DATA_ROW:
            Serial.println(F("Execute task INSERT_DATA_ROW"));
            break;
    }
}