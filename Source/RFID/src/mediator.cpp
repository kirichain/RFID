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
Buzzer buzzer;
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

    taskResults.currentFeature = NO_FEATURE;
    taskResults.currentTask = NO_TASK;
    taskResults.currentScreenItemIndex = 0;
    taskResults.featureNavigationHistory[++taskResults.featureNavigationHistorySize] = HOME_HANDHELD_2;

    Serial.println("Mediator initiated");
    //dataRow.timestamp = NULL;
}

void Mediator::init_services() {
    // Render layout based on operating mode
    if (taskArgs.feature == HOME_TERMINAL) {
        display.init(LANDSCAPE);
    } else {
        display.init(PORTRAIT);
    }
    display.render_feature(LOADING, taskResults);
    peripherals.init_navigation_buttons(leftUpNavButtonPinDefinition, backCancelNavButtonPinDefinition,
                                        menuSelectNavButtonPinDefinition, rightDownNavButtonPinDefinition);
    // Set buzzer pin
    peripherals.set_digital_output(buzzerPinDefinition);
    // Play welcome sound using buzzer
    buzzer.welcome_sound();
    // Check RFID module
    rfid.init(rfid_rx_pin, rfid_tx_pin);
    // Get mac address
    wifi.get_mac_addr();
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
        case READ_SERIAL_COMMUNICATION_MESSAGE:
            //Serial.println(F("Execute task RECEIVE_COMMUNICATION_MESSAGE"));

            break;
        case SEND_SERIAL_COMMUNICATION_MESSAGE:
            Serial.println(F("Execute task SEND_COMMUNICATION_MESSAGE"));

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
            mqtt.subscribe_topic(taskArgs.mqtt_subscribed_topic);
            break;
        case CONNECT_MQTT_BROKER:
            Serial.println(F("Execute task CONNECT_MQTT_BROKER"));
            mqtt.connect_to_broker(taskArgs.mqttBrokerIp, taskArgs.mqttBrokerPort, taskArgs.mqttLwtTopic,
                                   wifi.mac_address);
            break;
        case HANDLE_MQTT_MESSAGE:
            Serial.println(F("Execute task HANDLE_MQTT_MESSAGE"));
            mqtt.handle_incoming_message();
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
//            strncpy(taskArgs.wifi_sta_ssid, "ERPLTD", sizeof(taskArgs.wifi_sta_ssid));
//            strncpy(taskArgs.wifi_sta_password, "erp@@2020", sizeof(taskArgs.wifi_sta_password));
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
        case SCAN_WIFI_NETWORKS:
            Serial.println(F("Execute task SCAN_WIFI_NETWORKS"));
            wifi.scan_wifi_networks();
            taskResults.wifi_networks_count = wifi.wifi_networks_count;
            for (byte i = 0; i < 10; ++i) {
                taskResults.wifi_networks[i] = wifi.wifi_networks[i];
            }
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
            if (taskArgs.feature != taskResults.currentFeature) {
                Serial.print(F("Execute task RENDER_FEATURE :"));
                Serial.println(feature_as_string(taskArgs.feature));
                display.render_feature(taskArgs.feature, taskResults);
                // Check if this feature requires background tasks before rendering information, if yes, run tasks,
                // then re-render
                if (display.is_background_task_required) {
                    display.render_feature(LOADING, taskResults);
                    byte feature_background_task_index = 0;
                    while ((feature_background_task_index <= 9) and
                           (display.current_screen_background_tasks[feature_background_task_index] != NO_TASK)) {
                        Serial.println(F("Executing background task"));
                        execute_task(display.current_screen_background_tasks[feature_background_task_index]);
                        ++feature_background_task_index;
                    }

                    Serial.print(F("This feature has : "));
                    Serial.print(feature_background_task_index);
                    Serial.println(F(" background tasks"));

                    display.is_background_task_completed = true;
                    display.is_background_task_required = false;
                    display.render_feature(taskArgs.feature, taskResults);
                    display.is_background_task_completed = false;
                }
                // Update screen item index for screen selector
                taskResults.currentScreenItemIndex = 0;
                // Update screen item count for screen selector
                taskResults.screenItemCount = display.screen_item_count;
                // Update type of items on the screen
                taskResults.feature_item_type = display.current_feature_item_type;
                // Update list of features/tasks of items on the screen
                switch (taskResults.feature_item_type) {
                    case MENU_ICON:
                        for (int i = 0; i < 10; ++i) {
                            taskResults.screenFeatures[i] = display.current_screen_features[i];
                        }
                        break;
                    case LIST_ITEM:
                        taskResults.screenFeatures[0] = display.current_screen_features[0];
                        break;
                }
                // Print screen item count of this feature (screen)
                Serial.print(F("Feature has : "));
                Serial.print(taskResults.screenItemCount);
                Serial.println(F(" items on the screen"));
            } else {
                // Feature is not changed. Keep current rendering
                //Serial.println(F("Feature is not changed. Keep current rendering"));
            }
            break;
        case INIT_NAVIGATION_BUTTON:
            Serial.println(F("Execute task INIT_NAVIGATION_BUTTON"));
            peripherals.init_navigation_buttons(menuSelectNavButtonPinDefinition,
                                                leftUpNavButtonPinDefinition,
                                                rightDownNavButtonPinDefinition,
                                                backCancelNavButtonPinDefinition);
            break;
        case READ_NAVIGATION_BUTTON: {
            // Serial.println(F("Execute task READ_NAVIGATION_BUTTON"));
            // Get navigation direction
            // To store current screen item index with LIST_ITEM type
            byte previous_screen_item_index = taskResults.currentScreenItemIndex;
            button_type_t is_nav_button_pressed = peripherals.read_navigation_buttons(
                    taskResults.currentScreenItemIndex,
                    taskResults.screenItemCount,
                    taskResults.feature_item_type);
            // Clear current screen selector and update to new position from button state
            switch (is_nav_button_pressed) {
                case LEFT_UP:
                    // We just traverse through screen items for both cases
                    display.clear_screen_selector();
                    display.update_screen_selector(taskResults.currentScreenItemIndex);
                    if (display.current_feature_item_type == LIST_ITEM) {
                        // We traverse through screen items and update items on screen base on item index and page
                        if (taskResults.currentScreenItemIndex == 7 && previous_screen_item_index == 0) {
                            Serial.println(F("We re going up in the list"));
                            display.render_item_list(false, true, true, 40, 0x528B);
                        }
                    }
                    break;
                case RIGHT_DOWN:
                    // We just traverse through screen items for both cases
                    display.clear_screen_selector();
                    display.update_screen_selector(taskResults.currentScreenItemIndex);
                    if (display.current_feature_item_type == LIST_ITEM) {
                        // LIST_ITEM
                        // We traverse through screen items and update items on screen base on item index and page
                        if (taskResults.currentScreenItemIndex == 0 && previous_screen_item_index == 7) {
                            Serial.println(F("We re going down in the list"));
                            display.render_item_list(false, false, true, 40, 0x528B);
                        }
                    }
                    break;
                case SELECT:
                    switch (taskResults.feature_item_type) {
                        case MENU_ICON:
                            Serial.println(F("Retrieving corresponding feature now"));
                            Serial.print(F("Current screen item index: "));
                            Serial.println(taskResults.currentScreenItemIndex);
                            peripherals.retrieve_corresponding_feature(taskArgs.previousFeature,
                                                                       taskResults.currentFeature, taskArgs.feature,
                                                                       taskResults.currentScreenItemIndex,
                                                                       taskResults.screenFeatures,
                                                                       is_nav_button_pressed,
                                                                       taskResults.featureNavigationHistory,
                                                                       taskResults.featureNavigationHistorySize);

                            // Set screen selector border color accordingly to next feature
                            display.set_screen_selector_border_color(taskArgs.feature);
                            break;
                        case LIST_ITEM:
                            // When item is selected, start to switch to next screen and execute background task
                            //taskArgs.feature = taskResults.screenFeatures[0];
                            // Append selected item in the list of this screen into selected list items array
                            if (taskResults.selected_list_items[0] == "") {
                                taskResults.selected_list_items[0]
                                        = display.current_screen_list_items[taskResults.currentScreenItemIndex];
                            } else {
                                byte i = 9;
                                while (i > 0) {
                                    if (taskResults.selected_list_items[i - 1] != "") {
                                        taskResults.selected_list_items[i]
                                                = display.current_screen_list_items[taskResults.currentScreenItemIndex];
                                        break;
                                    }
                                    --i;
                                }
                            }

                            Serial.print(F("Submitted selected item in the list: "));
                            Serial.println(display.current_screen_list_items[taskResults.currentScreenItemIndex]);
                            // Set current item index to 0 because we only have 1 screen to be rendered next
                            taskResults.currentScreenItemIndex = 0;
                            Serial.println(F("Retrieving corresponding task now"));
                            peripherals.retrieve_corresponding_feature(taskArgs.previousFeature,
                                                                       taskResults.currentFeature, taskArgs.feature,
                                                                       taskResults.currentScreenItemIndex,
                                                                       taskResults.screenFeatures,
                                                                       is_nav_button_pressed,
                                                                       taskResults.featureNavigationHistory,
                                                                       taskResults.featureNavigationHistorySize);

                            Serial.println(F("Selected items from previous lists so far: "));
                            for (byte i = 0; i < 10; ++i) {
                                if (taskResults.selected_list_items[i] != "")
                                    Serial.println(taskResults.selected_list_items[i]);
                            }
                            //peripherals.retrieve_corresponding_task(taskArgs.previousTask, taskResults.currentTask);
                            break;
                    }
                    break;
                case BACK_CANCEL:
                    Peripherals::retrieve_corresponding_feature(taskArgs.previousFeature,
                                                                taskResults.currentFeature, taskArgs.feature,
                                                                taskResults.currentScreenItemIndex,
                                                                taskResults.screenFeatures, is_nav_button_pressed,
                                                                taskResults.featureNavigationHistory,
                                                                taskResults.featureNavigationHistorySize);

                    // Set screen selector border color accordingly to next feature
                    display.set_screen_selector_border_color(taskArgs.feature);
            }
            break;
        }
        case GET_CURRENT_FEATURE:
            Serial.println(F("Execute task GET_CURRENT_FEATURE"));
            taskArgs.feature = taskResults.currentFeature;
            break;
        case SET_CURRENT_FEATURE:
            Serial.println(F("Execute task SET_CURRENT_FEATURE"));
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
            //rfid.set_scanning_mode(SINGLE_SCAN);
            rfid.set_scanning_mode(MULTI_SCAN);
            rfid.scan_rfid_tag();
            break;
        case WRITE_RFID_TAG:
            Serial.println(F("Execute task WRITE_RFID_TAG"));
            break;
        case SET_RFID_SCANNING_MODE:
            Serial.println(F("Execute task SET_RFID_SCANNING_MODE"));
            rfid.set_scanning_mode(taskArgs.scanning_mode);
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
        case SUBMIT_CHOSEN_ITEM:
            Serial.println(F("Execute task SUBMIT_CHOSEN_ITEM"));
            break;
    }
}

void Mediator::set_current_task() {
    if (taskArgs.task != NO_TASK) {
        taskResults.currentTask = taskArgs.task;
        Serial.print(F("Set current task to: "));
        Serial.println(task_as_string(taskArgs.task));
    }
}

void Mediator::set_current_feature() {
    if (taskResults.currentFeature != taskArgs.feature) {
        taskResults.currentFeature = taskArgs.feature;
        Serial.print(F("Set current feature successfully to "));
        Serial.println(F(feature_as_string(taskResults.currentFeature)));
    }
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

feature_t Mediator::get_current_feature() const {
//    Serial.print(F("Current feature is: "));
//    Serial.println(F(feature_as_string(taskResults.currentFeature)));
    return taskResults.currentFeature;
}

const char *Mediator::task_as_string(task_t task) {
    return task_names[task];
}

const char *Mediator::feature_as_string(feature_t feature) {
    return feature_names[feature];
}