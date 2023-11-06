//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_MEDIATOR_H
#define RFID_MEDIATOR_H

#include "Arduino.h"
#include "structs.h"
#include "message_queue.h"
#include "battery.h"
#include "buzzer.h"
#include "config.h"
#include "conveyor.h"
#include "data_export.h"
#include "data_import.h"
#include "display.h"
#include "fs_esp32.h"
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
#include "web_server.h"
#include "wifi.h"
#include "ws2812b.h"

class Mediator {
public:
    bool isTaskExecutable, isTaskCompleted, isTaskQueueEmpty;

    task_args taskArgs;
    task_results taskResults;

    Mediator();

    void execute_task(task_t task);

    void set_current_task();

    void set_current_feature();

    void set_current_task_status(bool taskStatus);

    bool get_current_task_status();

    task_t get_current_task();

    feature_t get_current_feature();

    char* task_as_string(task_t task);

    char* feature_as_string(feature_t feature);
};

#endif //RFID_MEDIATOR_H
