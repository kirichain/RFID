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
#include "wifi_esp32.h"
#include "ws2812b.h"

class Mediator {
private:
    const char *task_names[NUM_TASKS] = {
            "IDLE", "BLINK_LED", "BLINK_SCREEN", "INIT_MESSAGE_QUEUE", "CLEAR_MESSAGE_QUEUE",
            "PUBLISH_MQTT_MESSAGE", "SUBSCRIBE_MQTT_TOPIC", "RETRIEVE_MQTT_MESSAGE", "CONNECT_MQTT_BROKER",
            "HANDLE_MQTT_MESSAGE", "LOAD_CONFIG", "SAVE_CONFIG", "LOAD_FS", "SAVE_FS", "CHECK_CONNECTION",
            "INIT_AP_WIFI", "INIT_STA_WIFI", "TERMINATE_AP_WIFI", "TERMINATE_STA_WIFI", "GET_OPERATING_MODE",
            "SET_OPERATING_MODE", "RENDER_FEATURE", "GET_CURRENT_FEATURE", "READ_NAVIGATION_BUTTON",
            "INIT_NAVIGATION_BUTTON", "SET_CURRENT_FEATURE", "SET_TASK", "SET_TASK_STATUS", "GET_TASK_STATUS",
            "GET_RFID_SCAN_DATA", "IMPORT_DATA_FROM_SD_CARD", "IMPORT_DATA_FROM_SERVER", "EXPORT_DATA_TO_SD_CARD",
            "EXPORT_DATA_TO_SERVER", "SYNC_DATA_TO_SERVER", "SYNC_DATA_TO_DEVICE", "READ_RFID_TAG",
            "WRITE_RFID_TAG", "SET_RFID_SCANNING_MODE", "INSERT_DATA_ROW", "UPDATE_DATA_ROW", "DELETE_DATA_ROW",
            "SAVE_DATA_COLLECTION", "DELETE_DATA_COLLECTION", "LOAD_DATA_COLLECTION",
            "READ_PERIPHERAL_INPUT", "SEND_PERIPHERAL_OUTPUT", "START_CONVEYOR", "STOP_CONVEYOR"
    };
    const char *feature_names[NUM_FEATURES] = {
            "BOOT", "HOME_HANDHELD_1", "HOME_HANDHELD_2", "HOME_TERMINAL", "SETUP", "SETUP_WIFI",
            "SETUP_USER_INFO", "SETUP_USER_INFO_LOGIN", "SETUP_USER_INFO_LOGOUT", "RFID",
            "RFID_SCAN", "RFID_SCAN_HISTORY", "RFID_SCAN_RESULT", "RFID_MODIFY_TAG_DATA",
            "PACKAGE", "PACKAGE_DETAILS", "CO_WORKING", "CO_WORKING_SCAN_NEARBY_DEVICE",
            "CO_WORKING_CONNECT_TO_DEVICE", "CO_WORKING_CONNECT_TO_SERVER", "CO_WORKING_RUNNING",
            "DATABASE", "DATA_IMPORT", "DATA_IMPORT_FROM_SD_CARD", "DATA_IMPORT_FROM_SERVER",
            "DATA_EXPORT", "DATA_EXPORT_TO_SERVER", "DATA_EXPORT_TO_SD_CARD", "DATA_SYNC",
            "DATA_SYNC_TO_SERVER", "DATA_SYNC_TO_DEVICE", "NO_FEATURE"
    };
public:
    bool isTaskExecutable, isTaskCompleted, isTaskQueueEmpty;

    task_args taskArgs{};
    task_results taskResults{};

    Mediator();

    void init_services() const;

    void execute_task(task_t task);

    void set_current_task();

    void set_current_feature();

    void set_current_task_status(bool taskStatus);

    bool get_current_task_status() const;

    task_t get_current_task();

    feature_t get_current_feature();

    const char *task_as_string(task_t _task);

    const char *feature_as_string(feature_t _feature);
};

#endif //RFID_MEDIATOR_H
