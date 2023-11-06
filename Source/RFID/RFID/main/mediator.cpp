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

    taskArgs.feature = HOME_TERMINAL;
    taskArgs.task = IDLE;

    //dataRow.timestamp = NULL;
}


void Mediator::execute_task(task_t task) {
    switch (task) {
        case BLINK_LED:
            Serial.println(F("Execute task BLINK_LED"));
            peripherals.blink_led(taskArgs.blinkLedPin);
            break;
        case INIT_MESSAGE_QUEUE:
            Serial.println(F("Execute task INIT_MESSAGE_QUEUE"));
            break;
        case CLEAR_MESSAGE_QUEUE:
            Serial.println(F("Execute task CLEAR_MESSAGE_QUEUE"));
            break;
        case PUBLISH_MESSAGE:
            Serial.println(F("Execute task PUBLISH_MESSAGE"));
            break;
        case SUBSCRIBE_TOPIC:
            Serial.println(F("Execute task SUBSCRIBE_TOPIC"));
            break;
        case RETRIEVE_MESSAGE:
            Serial.println(F("Execute task RETRIEVE_MESSAGE"));
            break;
        case CONNECT_MQTT_BROKER:
            Serial.println(F("Execute task CONNECT_MQTT_BROKER"));
            mqtt.isBrokerConnected = false;
            while (!mqtt.isBrokerConnected) {
                mqtt.connect_to_broker(taskArgs.mqttBrokerUrl);
            }
            break;
        case SUBSCRIBE_MQTT_TOPIC:
            Serial.println(F("Execute task SUBSCRIBE_MQTT_TOPIC"));
            break;
        case PUBLISH_MQTT_MESSAGE:
            Serial.println(F("Execute task PUBLISH_MQTT_MESSAGE"));
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
            break;
        case INIT_STA_WIFI:
            Serial.println(F("Execute task INIT_STA_WIFI"));
            break;
        case TERMINATE_AP_WIFI:
            Serial.println(F("Execute task TERMINATE_AP_WIFI"));
            break;
        case TERMINATE_STA_WIFI:
            Serial.println(F("Execute task TERMINATE_STA_WIFI"));
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
            if (taskArgs.feature != taskResults.currentFeature) {
                display.render_feature(taskArgs.feature);
            } else {
                Serial.println(F("Feature is not changed. Keep current rendering"));
            }
            break;
        case INIT_NAVIGATION_BUTTON:
            Serial.println(F("Execute task INIT_NAVIGATION_BUTTON"));
            peripherals.init_navigation_buttons(menuSelectNavButton, leftUpNavButton, rightDownNavButton);
            break;
        case READ_NAVIGATION_BUTTON:
            Serial.println(F("Execute task READ_NAVIGATION_BUTTON"));
            peripherals.read_navigation_buttons(menuSelectNavButton, leftUpNavButton, rightDownNavButton);
            taskArgs.previousFeature = taskResults.currentFeature;
            taskArgs.previousTask = taskResults.currentTask;
            taskResults.currentFeature = peripherals.retrieve_corresponding_feature(taskResults.currentFeature);
            taskResults.currentTask = peripherals.retrieve_corresponding_task(taskResults.currentTask);
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
    Serial.println(F("Set current task successfuly to "));

}

void Mediator::set_current_feature() {
    taskResults.currentFeature = taskArgs.feature;
    Serial.println(F("Set current feature successfuly to "));

}

void Mediator::set_current_task_status(bool taskStatus) {
    if (taskStatus) {
        Serial.println(F("Set task status successfuly to completed"));
    } else {
        Serial.println(F("Set task status successfuly to incompleted"));
    }
    isTaskCompleted = taskStatus;
    isTaskExecutable = true;
    //isTaskExecutable = !taskStatus;
}

bool Mediator::get_current_task_status() {
    return isTaskCompleted;
}

task_t Mediator::get_current_task() {
    return taskResults.currentTask;
}

feature_t Mediator::get_current_feature() {
    return taskResults.currentFeature;
}

char* Mediator::task_as_string(task_t task) {
    char* taskAsString = "";
    return taskAsString;
}

char* Mediator::feature_as_string(feature_t feature) {
    char* featureAsString = nullptr;
    return featureAsString;
}