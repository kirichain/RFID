//
// Created by Tan on 25-Oct-23.
//
#include "Arduino.h"
#include "mediator.h"

Mediator mediator;

// Serial: For debugging
// Serial1: For communicating with rfid module
// Serial3: For communicating with computer
void setup() {
    Serial.begin(115200);
    mediator.init_services();
//    mediator.taskArgs.feature = BOOT;
//    mediator.set_current_feature();
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.execute_task(LOAD_FS);
//    if ((mediator.taskResults.isFsLoaded)) {
//        mediator.taskArgs.operatingMode = mediator.taskResults.savedOperatingModeInFs;
//        if (mediator.taskResults.savedOperatingModeInFs == HANDHELD) {
//            mediator.taskArgs.feature = HOME_HANDHELD_1;
//        } else {
//            mediator.taskArgs.feature = HOME_TERMINAL;
//        }
//    } else {
//        mediator.taskArgs.operatingMode = HANDHELD;
//    }
//    mediator.set_current_feature();
//    mediator.execute_task(RENDER_FEATURE);

//    mediator.taskArgs.operatingMode = HANDHELD;
//    mediator.execute_task(SET_OPERATING_MODE);
    mediator.taskArgs.feature = HOME_HANDHELD_1;
    mediator.execute_task(RENDER_FEATURE);
    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = SETTING;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = SETTING_USER_INFO;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = HOME_HANDHELD_1;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = RFID;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = RFID_SCAN_RESULT;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = RFID_SCAN;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = RFID_SCAN_HISTORY;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = CO_WORKING;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = HOME_HANDHELD_1;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = DATABASE;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = DATA_IMPORT;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = HOME_HANDHELD_1;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = PACKAGE;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = HOME_HANDHELD_1;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.taskArgs.feature = RFID_SCAN_RESULT;
//    mediator.execute_task(RENDER_FEATURE);
//    mediator.set_current_feature();
//    delay(4000);
//    mediator.execute_task(CHECK_CONNECTION);
//    mediator.execute_task(INIT_AP_WIFI);
//    mediator.execute_task(INIT_STA_WIFI);

//For testing, we execute task BLINK_LED and stop this task when we receive message from MQTT broker
//    mediator.taskArgs.task = BLINK_SCREEN;
//    mediator.set_current_task();
//    mediator.set_current_task_status(false);
//    mediator.execute_task(mediator.taskArgs.task);
//    mediator.taskArgs.task = INIT_STA_WIFI;
//    mediator.set_current_task();
//    mediator.set_current_task_status(false);
//    mediator.execute_task(mediator.taskArgs.task);
}

void loop() {
    mediator.get_current_feature();
    mediator.execute_task(RENDER_FEATURE);
    mediator.set_current_feature();
    mediator.execute_task(READ_NAVIGATION_BUTTON);
//    mediator.taskArgs.task = mediator.taskResults.currentTask;
//    mediator.set_current_task();
//    if (mediator.isTaskExecutable) {
//        while ((!mediator.isTaskCompleted) & (!mediator.isTaskQueueEmpty)) {
//            mediator.taskArgs.task = mediator.taskResults.currentTask;
//            mediator.execute_task(mediator.taskArgs.task);
//            mediator.execute_task(READ_NAVIGATION_BUTTON);
//            mediator.execute_task(SUBSCRIBE_MQTT_TOPIC);
//            mediator.get_current_task_status();
//            yield();
//        }
//
//        Serial.println(F("Task execution completed"));
//        mediator.set_current_task_status(true);
//        mediator.taskArgs.task = IDLE;
//        mediator.set_current_task();
//        mediator.set_current_task_status(false);
//    }
}