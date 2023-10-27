//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_MEDIATOR_H
#define RFID_MEDIATOR_H

#include "Arduino.h"
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

enum operating_mode {
    TERMINAL,
    HANDHELD,
};

enum feature {
    SETUP,
    SETUP_WIFI,
    SETUP_USER_INFO,
    SETUP_USER_INFO_LOGIN,
    SETUP_USER_INFO_LOGOUT,
    RFID,
    RFID_SCAN,
    RFID_SCAN_HISTORY,
    PACKAGE,
    PACKAGE_DETAILS,
    CO_WORKING,
    CO_WORKING_SCAN_NEARBY_DEVICE,
    CO_WORKING_CONNECT_TO_DEVICE,
    CO_WORKING_CONNECT_TO_SERVER,
    CO_WORKING_SCAN,
    DATABASE,
    DATA_IMPORT,
    DATA_IMPORT_FROM_SD_CARD,
    DATA_IMPORT_FROM_SERVER,
    DATA_EXPORT,
    DATA_EXPORT_TO_SERVER,
    DATA_EXPORT_TO_SD_CARD,
};

enum task {
    CHECK_FS,
    LOAD_FS,
    CHECK_CONNECTION,
    INIT_AP_WIFI,
    INIT_STA_WIFI,
    GET_OPERATING_MODE,
    SET_OPERATING_MODE,
    RENDER_OPERATING_MODE,
    RENDER_FEATURE,
    GET_FEATURE,
    GET_NAVIGATION,
    SET_FEATURE,
    SET_FEATURE_TASK,
    SET_FEATURE_TASK_STATUS,
    GET_FEATURE_TASK_STATUS,
    GET_RFID_SCAN_DATA,
    EXPORT_DATA_TO_SD_CARD,
    EXPORT_DATA_TO_SERVER,
    SCAN_RFID_TAG,
    INSERT_DATA_ROW,
    UPDATE_DATA_ROW,
    DELETE_DATA_ROW,
};

typedef struct task_args {
    String function;
} task_args;

feature currentFeature;
task currentTask;
task_args taskArgs;

bool isTaskExecutable;
bool isTaskCompleted;

class Mediator {
public:
    Mediator();

    void get();

    void set();

    static void execute_task(task task);
};

#endif //RFID_MEDIATOR_H
