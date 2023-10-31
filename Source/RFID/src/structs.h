//
// Created by Tan on 31-Oct-23.
//

#ifndef RFID_STRUCTS_H
#define RFID_STRUCTS_H

#include "enums.h"

typedef struct data_row {
    int timestamp;
} data_row;

typedef struct data_collection {
    data_row _row;
} data_collection;

typedef struct task_args {
    task_t task, previousTask;
    feature_t feature, previousFeature;
    operating_mode operatingMode;
    char *wifi_ap_ssid;
    char *wifi_ap_password;
    char *wifi_sta_ssid;
    char *wifi_sta_password;
    data_row dataRow;
    data_collection dataCollection;
    char* mqttTopic;
    char* mqttBrokerUrl;
} task_args;

typedef struct task_results {
    bool isFsLoaded;
    operating_mode savedOperatingModeInFs;
    operating_mode currentOperatingMode;
    feature_t currentFeature;
    task_t currentTask;
} task_results;

typedef struct message {
    int timestamp;
    char *content;
} message;

typedef struct device_config {
    char* defaultApWifiSSID;
    char* defaultApWifiPassword;
    char* defaultStaWifiSSID;
    char* defaultStaWifiPassword;
} device_config;
#endif //RFID_STRUCTS_H
