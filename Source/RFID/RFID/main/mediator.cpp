//
// Created by Tan on 25-Oct-23.
//

#include "mediator.h"

Ws2812b ws2812b;
Wifi wifi;
DataExport dataExport;
DataImport dataImport;
Request request;
Conveyor conveyor;
Display display;
FsEsp32 fsEsp32;
User user;
IAM iam;
MeshNetwork meshNetwork;
Operation operation;
Peripherals peripherals;
Rfid rfid;
Socket socket;
Warehouse warehouse;
Package package;
SdCard sdCard;
QrCodeScanner qrCodeScanner;
QrCode qrCode;
OTA ota;
MessageQueue messageQueue;
MQTT mqtt;
WebServer server;
Queue taskQueue;

Mediator::Mediator() {
    isTaskExecutable = false;
    isTaskCompleted = true;
    isTaskQueueEmpty = true;

    taskResults.currentFeature = HOME_HANDHELD_1;
    taskArgs.task = IDLE;

    Serial.println("Mediator initiated");
    //dataRow.timestamp = NULL;
}

void Mediator::init_services() const {
    if (taskArgs.feature == HOME_TERMINAL) {
        display.init(LANDSCAPE);
    } else {
        display.init(PORTRAIT);
    }
}

void Mediator::execute_task(task_t task) {
    isTaskExecutable = true;
    isTaskQueueEmpty = false;
    switch (task) {
        case IDLE:
            //Do nothing in idle mode until nav button is pressed
            //isTaskCompleted = true;
            break;
        case BLINK_LED:
            Serial.println(F("Execute task BLINK_LED"));
            peripherals.blink_led(taskArgs.blinkLedPin);
            isTaskCompleted = true;
            break;
        case BLINK_SCREEN:
            Serial.println(F("Execute task BLINK_SCREEN"));
            display.blink_screen(isTaskCompleted);
            break;
        case INIT_MESSAGE_QUEUE:
            Serial.println(F("Execute task INIT_MESSAGE_QUEUE"));
            break;
        case CLEAR_MESSAGE_QUEUE:
            Serial.println(F("Execute task CLEAR_MESSAGE_QUEUE"));
            break;
        case PUBLISH_MQTT_MESSAGE:
            Serial.println(F("Execute task PUBLISH_MQTT_MESSAGE"));
            break;
        case SUBSCRIBE_MQTT_TOPIC:
            Serial.println(F("Execute task SUBSCRIBE_TOPIC"));
            break;
        case RETRIEVE_MQTT_MESSAGE:
            Serial.println(F("Execute task RETRIEVE_MQTT_MESSAGE"));
            break;
        case CONNECT_MQTT_BROKER:
            Serial.println(F("Execute task CONNECT_MQTT_BROKER"));
            mqtt.isBrokerConnected = false;
            while (!mqtt.isBrokerConnected) {
                mqtt.connect_to_broker(taskArgs.mqttBrokerUrl);
            }
            break;
        case HANDLE_MQTT_MESSAGE:
            Serial.println(F("Execute task HANDLE_MQTT_MESSAGE"));
            break;
        case LOAD_CONFIG:
            Serial.println(F("Execute task LOAD_CONFIG"));
            break;
        case SAVE_CONFIG:
            Serial.println(F("Execute task SAVE_CONFIG"));
            break;
        case LOAD_FS:
            Serial.println(F("Execute task LOAD_FS"));
            break;
        case SAVE_FS:
            Serial.println(F("Execute task SAVE_FS"));
            break;
        case CHECK_CONNECTION:
            Serial.println(F("Execute task CHECK_CONNECTION"));
            break;
        case INIT_AP_WIFI:
            Serial.println(F("Execute task INIT_AP_WIFI"));
            strncpy(taskArgs.wifi_ap_ssid, "RFID-001", sizeof(taskArgs.wifi_ap_ssid));
            strncpy(taskArgs.wifi_ap_password, "rfid001x", sizeof(taskArgs.wifi_ap_password));
            // Ensure null-termination if the string length equals the buffer size
            taskArgs.wifi_ap_ssid[sizeof(taskArgs.wifi_ap_ssid) - 1] = '\0';
            taskArgs.wifi_ap_password[sizeof(taskArgs.wifi_ap_password) - 1] = '\0';
            wifi.set_ap_wifi_credential(taskArgs.wifi_ap_ssid, taskArgs.wifi_ap_password);
            // Start to connect to Wi-Fi as AP credential
            if (wifi.init_ap_mode()) {
                Serial.println(F("Init ap wifi successfully"));
            }
            break;
        case INIT_STA_WIFI:
            Serial.println(F("Execute task INIT_STA_WIFI"));
            strncpy(taskArgs.wifi_sta_ssid, "SFS OFFICE", sizeof(taskArgs.wifi_sta_ssid));
            strncpy(taskArgs.wifi_sta_password, "sfs#office!@", sizeof(taskArgs.wifi_sta_password));
            strncpy(taskArgs.wifi_hostname, device_hostname, sizeof(taskArgs.wifi_hostname));
            // Ensure null-termination if the string length equals the buffer size
            taskArgs.wifi_sta_ssid[sizeof(taskArgs.wifi_sta_ssid) - 1] = '\0';
            taskArgs.wifi_sta_password[sizeof(taskArgs.wifi_sta_password) - 1] = '\0';
            taskArgs.wifi_hostname[sizeof(taskArgs.wifi_hostname) - 1] = '\0';
            wifi.set_sta_wifi_credential(taskArgs.wifi_sta_ssid, taskArgs.wifi_sta_password, taskArgs.wifi_hostname);
            // Start to connect to Wi-Fi as STA credential
            if (wifi.init_sta_mode()) {
                Serial.println(F("Init sta wifi successfully"));
            } else {
                Serial.println(F("Init sta wifi failed"));
            }
            break;
        case TERMINATE_AP_WIFI:
            Serial.println(F("Execute task TERMINATE_AP_WIFI"));
            wifi.terminate_ap_mode();
            break;
        case TERMINATE_STA_WIFI:
            Serial.println(F("Execute task TERMINATE_STA_WIFI"));
            wifi.terminate_sta_mode();
            break;
        case GET_OPERATING_MODE:
            Serial.println(F("Execute task GET_OPERATING_MODE"));
            taskResults.currentOperatingMode = operation.get_operating_mode();
            break;
        case SET_OPERATING_MODE:
            Serial.println(F("Execute task SET_OPERATING_MODE"));
            operation.set_operation_mode(taskArgs.operatingMode);
            taskResults.currentOperatingMode = taskArgs.operatingMode;
            break;
        case RENDER_FEATURE:
            Serial.println(F("Execute task RENDER_FEATURE"));
            //if (taskArgs.feature != taskResults.currentFeature) {
            display.render_feature(taskArgs.feature);
            //} else {
            //    Serial.println(F("Feature is not changed. Keep current rendering"));
            //}
            break;
        case INIT_NAVIGATION_BUTTON:
            Serial.println(F("Execute task INIT_NAVIGATION_BUTTON"));
            peripherals.init_navigation_buttons(menuSelectNavButtonPinDefinition,
                                                leftUpNavButtonPinDefinition,
                                                rightDownNavButtonPinDefinition,
                                                backCancelNavButtonPinDefinition);
            break;
        case READ_NAVIGATION_BUTTON:
            Serial.println(F("Execute task READ_NAVIGATION_BUTTON"));
            peripherals.read_navigation_buttons();
            peripherals.retrieve_corresponding_feature(taskArgs.previousFeature, taskResults.currentFeature);
            peripherals.retrieve_corresponding_task(taskArgs.previousTask, taskResults.currentTask);
            break;
        case GET_FEATURE:
            Serial.println(F("Execute task GET_FEATURE"));
            taskArgs.feature = taskResults.currentFeature;
            break;
        case SET_FEATURE:
            Serial.println(F("Execute task SET_FEATURE"));
            if (taskArgs.feature != taskResults.currentFeature) {
                taskArgs.previousFeature = taskResults.currentFeature;
                taskResults.currentFeature = taskArgs.feature;
            }
            break;
        case SET_TASK:
            Serial.println(F("Execute task SET_TASK"));
            taskResults.currentTask = taskArgs.task;
            break;
        case SET_TASK_STATUS:
            Serial.println(F("Execute task SET_TASK_STATUS"));
            break;
        case GET_TASK_STATUS:
            Serial.println(F("Execute task GET_TASK_STATUS"));
            break;
        case GET_RFID_SCAN_DATA:
            Serial.println(F("Execute task GET_RFID_SCAN_DATA"));
            break;
        case IMPORT_DATA_FROM_SD_CARD:
            Serial.println(F("Execute task IMPORT_DATA_FROM_SD_CARD"));
            break;
        case IMPORT_DATA_FROM_SERVER:
            Serial.println(F("Execute task IMPORT_DATA_FROM_SERVER"));
            break;
        case EXPORT_DATA_TO_SD_CARD:
            Serial.println(F("Execute task EXPORT_DATA_TO_SD_CARD"));
            break;
        case EXPORT_DATA_TO_SERVER:
            Serial.println(F("Execute task EXPORT_DATA_TO_SERVER"));
            break;
        case SYNC_DATA_TO_SERVER:
            Serial.println(F("Execute task SYNC_DATA_TO_SERVER"));
            break;
        case SYNC_DATA_TO_DEVICE:
            Serial.println(F("Execute task SYNC_DATA_TO_DEVICE"));
            break;
        case READ_RFID_TAG:
            Serial.println(F("Execute task READ_RFID_TAG"));
            break;
        case WRITE_RFID_TAG:
            Serial.println(F("Execute task WRITE_RFID_TAG"));
            break;
        case INSERT_DATA_ROW:
            Serial.println(F("Execute task INSERT_DATA_ROW"));
            break;
        case UPDATE_DATA_ROW:
            Serial.println(F("Execute task UPDATE_DATA_ROW"));
            break;
        case DELETE_DATA_ROW:
            Serial.println(F("Execute task DELETE_DATA_ROW"));
            break;
        case SAVE_DATA_COLLECTION:
            Serial.println(F("Execute task SAVE_DATA_COLLECTION"));
            break;
        case DELETE_DATA_COLLECTION:
            Serial.println(F("Execute task DELETE_DATA_COLLECTION"));
            break;
        case LOAD_DATA_COLLECTION:
            Serial.println(F("Execute task LOAD_DATA_COLLECTION"));
            break;
        case READ_PERIPHERAL_INPUT:
            Serial.println(F("Execute task READ_PERIPHERAL_INPUT"));
            break;
        case SEND_PERIPHERAL_OUTPUT:
            Serial.println(F("Execute task SEND_PERIPHERAL_OUTPUT"));
            break;
        case START_CONVEYOR:
            Serial.println(F("Execute task START_CONVEYOR"));
            break;
        case STOP_CONVEYOR:
            Serial.println(F("Execute task STOP_CONVEYOR"));
            break;
    }
}

void Mediator::set_current_task() {
    taskResults.currentTask = taskArgs.task;
    Serial.print(F("Set current task to: "));
    Serial.println(task_as_string(taskArgs.task));
}

void Mediator::set_current_feature() {
    taskResults.currentFeature = taskArgs.feature;
    Serial.println(F("Set current feature successfully to "));

}

void Mediator::set_current_task_status(bool taskStatus) {
    if (taskStatus) {
        Serial.println(F("Set current task status to completed"));
    } else {
        Serial.println(F("Set current task status to incomplete"));
    }
    isTaskCompleted = taskStatus;
    isTaskExecutable = !taskStatus;
}

bool Mediator::get_current_task_status() const {
    if (isTaskCompleted) {
        Serial.println(F("Current task status: completed"));
    } else {
        Serial.println(F("Current task status: incomplete"));
    }
    return isTaskCompleted;
}

task_t Mediator::get_current_task() {
    Serial.print(F("Current task is: "));
    Serial.println(task_as_string(taskResults.currentTask));
    return taskResults.currentTask;
}

feature_t Mediator::get_current_feature() {
    Serial.print(F("Current feature is: "));
    Serial.println(feature_as_string(taskResults.currentFeature));
    return taskResults.currentFeature;
}

const char *Mediator::task_as_string(task_t task) {
    return task_names[task];
}

const char *Mediator::feature_as_string(feature_t feature) {
    return feature_names[feature];
}