//
// Created by Tan on 28-Oct-23.
//

#ifndef RFID_ENUMS_H
#define RFID_ENUMS_H

enum operating_mode_t {
    TERMINAL,
    HANDHELD,
};

enum feature_layout_t {
    PORTRAIT,
    LANDSCAPE
};

enum feature_t {
    BOOT,
    HOME_HANDHELD_1,
    HOME_HANDHELD_2,
    HOME_TERMINAL,
    SETTING,
    SETTING_WIFI,
    SETTING_USER_INFO,
    SETTING_USER_INFO_LOGIN,
    SETTING_USER_INFO_LOGOUT,
    RFID,
    RFID_SCAN,
    RFID_SCAN_HISTORY,
    RFID_SCAN_RESULT,
    RFID_MODIFY_TAG_DATA,
    PACKAGE,
    PACKAGE_DETAILS,
    CO_WORKING,
    CO_WORKING_SCAN_NEARBY_DEVICE,
    CO_WORKING_CONNECT_TO_DEVICE,
    CO_WORKING_CONNECT_TO_SERVER,
    CO_WORKING_RUNNING,
    DATABASE,
    DATA_IMPORT,
    DATA_IMPORT_FROM_SD_CARD,
    DATA_IMPORT_FROM_SERVER,
    DATA_IMPORT_FROM_COMPUTER,
    DATA_EXPORT,
    DATA_EXPORT_TO_SERVER,
    DATA_EXPORT_TO_SD_CARD,
    DATA_SYNC,
    DATA_SYNC_TO_SERVER,
    DATA_SYNC_TO_DEVICE,
    NO_FEATURE,
    NUM_FEATURES
};

enum task_t {
    IDLE,
    BLINK_LED,
    BLINK_SCREEN,
    INIT_MESSAGE_QUEUE,
    CLEAR_MESSAGE_QUEUE,
    PUBLISH_MQTT_MESSAGE,
    SUBSCRIBE_MQTT_TOPIC,
    RETRIEVE_MQTT_MESSAGE,
    CONNECT_MQTT_BROKER,
    HANDLE_MQTT_MESSAGE,
    LOAD_CONFIG,
    SAVE_CONFIG,
    LOAD_FS,
    SAVE_FS,
    CHECK_CONNECTION,
    INIT_AP_WIFI,
    INIT_STA_WIFI,
    TERMINATE_AP_WIFI,
    TERMINATE_STA_WIFI,
    GET_OPERATING_MODE,
    SET_OPERATING_MODE,
    RENDER_FEATURE,
    GET_CURRENT_FEATURE,
    READ_NAVIGATION_BUTTON,
    INIT_NAVIGATION_BUTTON,
    SET_CURRENT_FEATURE,
    SET_TASK,
    SET_TASK_STATUS,
    GET_TASK_STATUS,
    GET_RFID_SCAN_DATA,
    IMPORT_DATA_FROM_SD_CARD,
    IMPORT_DATA_FROM_SERVER,
    EXPORT_DATA_TO_SD_CARD,
    EXPORT_DATA_TO_SERVER,
    SYNC_DATA_TO_SERVER,
    SYNC_DATA_TO_DEVICE,
    READ_RFID_TAG,
    WRITE_RFID_TAG,
    SET_RFID_SCANNING_MODE,
    INSERT_DATA_ROW,
    UPDATE_DATA_ROW,
    DELETE_DATA_ROW,
    SAVE_DATA_COLLECTION,
    DELETE_DATA_COLLECTION,
    LOAD_DATA_COLLECTION,
    READ_PERIPHERAL_INPUT,
    SEND_PERIPHERAL_OUTPUT,
    START_CONVEYOR,
    STOP_CONVEYOR,
    NO_TASK,
    NUM_TASKS
};

enum feature_item_type_t {
    MENU_ICON,
    LIST_ITEM
};

enum button_type_t {
    LEFT_UP,
    BACK_CANCEL,
    SELECT,
    RIGHT_DOWN,
    NOT_PRESSED
};

enum rfid_tag_status_t {
    ASSOCIATED,
    UNASSOCIATED
};

enum rfid_scanning_mode_t {
    SINGLE_SCAN,
    MULTI_SCAN
};

enum screen_selector_t {
    MENU_SELECTING,
    ITEM_LIST_SELECTING
};
#endif //RFID_ENUMS_H
