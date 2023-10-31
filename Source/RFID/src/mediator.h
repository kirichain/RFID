//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_MEDIATOR_H
#define RFID_MEDIATOR_H

#include "Arduino.h"
#include "enums.h"
#include "message_queue.h"
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
#include "mqtt.h"
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
#include "queue.h"

typedef struct task_args {
    task task;
    feature feature;
    operating_mode operatingMode;
    char *wifi_ap_ssid;
    char *wifi_ap_password;
    char *wifi_sta_ssid;
    char *wifi_sta_password;
    data_row dataRow;
    data_collection dataCollection;
} task_args;

typedef struct task_results {
    bool isFsLoaded;
    operating_mode savedOperatingModeInFs;
    operating_mode currentOperatingMode;
    feature currentFeature;
    task currentTask;
} task_results;


class Mediator {
public:
    bool isTaskExecutable, isTaskCompleted;

    task_args taskArgs;
    task_results taskResults;

    Mediator();

    void execute_task(task task);
};

#endif //RFID_MEDIATOR_H
