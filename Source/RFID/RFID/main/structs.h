//
// Created by Tan on 31-Oct-23.
//

#ifndef RFID_STRUCTS_H
#define RFID_STRUCTS_H

#include "enums.h"

#define WIFI_STATUS_MAX_LEN 50
#define DATE_TIME_MAX_LEN   30
#define SERVER_STATUS_MAX_LEN 50
#define LOGIN_STATUS_MAX_LEN  50

typedef struct {
    const char *name;
    const uint16_t *icon_data;
} menu_icon;

typedef struct {
    char wifiStatus[WIFI_STATUS_MAX_LEN];
    char dateTime[DATE_TIME_MAX_LEN];
    char serverStatus[SERVER_STATUS_MAX_LEN];
    char loginStatus[LOGIN_STATUS_MAX_LEN];
} status_indicators;

typedef struct data_row {
    int timestamp;
} data_row;

typedef struct data_collection {
    data_row _row;
} data_collection;

typedef struct task_args {
    task_t task, previousTask;
    feature_t feature, previousFeature;
    operating_mode_t operatingMode;
    char wifi_ap_ssid[32];
    char wifi_ap_password[32];
    char wifi_sta_ssid[32];
    char wifi_sta_password[32];
    char wifi_hostname[32];
    data_row dataRow;
    data_collection dataCollection;
    char *mqttTopic;
    char *mqttBrokerUrl;
    byte blinkLedPin;
} task_args;

typedef struct task_results {
    bool isFsLoaded;
    operating_mode_t savedOperatingModeInFs;
    operating_mode_t currentOperatingMode;
    feature_t currentFeature;
    task_t currentTask;
} task_results;

typedef struct message {
    int timestamp;
    char *content;
} message;

typedef struct device_config {
    char *defaultApWifiSSID;
    char *defaultApWifiPassword;
    char *defaultStaWifiSSID;
    char *defaultStaWifiPassword;
} device_config;

typedef struct rfid_tag {
    String epc;
    String productId;
    String productSize;
    String productColor;
    String productImgUrl;
    String shippedToId;
    String brandName;
    String poCode;
    rfid_tag_status_t tagStatus;
} rfid_tag;

typedef struct rfid_item {
    String item_id;
    String description;
    rfid_tag rfid_data;
} rfid_item;

typedef struct rfid_scan_result {
    bool success;
    rfid_tag scan_data;
    //String epc;
    String timestamp;
    byte scan_count;
} rfid_scan_result;

#endif //RFID_STRUCTS_H
