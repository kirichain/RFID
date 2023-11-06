//
// Created by Tan on 25-Oct-23.
//
#include "Arduino.h"
#include "mediator.h"

Mediator mediator;

void setup() {
    mediator.taskArgs.feature = BOOT;
    mediator.set_current_feature();
    mediator.execute_task(RENDER_FEATURE);
    mediator.execute_task(LOAD_FS);
    if ((mediator.taskResults.isFsLoaded)) {
        mediator.taskArgs.operatingMode = mediator.taskResults.savedOperatingModeInFs;
        if (mediator.taskResults.savedOperatingModeInFs == HANDHELD) {
            mediator.taskArgs.feature = HOME_HANDHELD;
        } else {
            mediator.taskArgs.feature = HOME_TERMINAL;
        }
        mediator.set_current_feature();
    } else {
        mediator.taskArgs.operatingMode = HANDHELD;
    }
    mediator.execute_task(SET_OPERATING_MODE);
    mediator.execute_task(CHECK_CONNECTION);
    mediator.execute_task(INIT_AP_WIFI);
    mediator.execute_task(INIT_STA_WIFI);
}

void loop() {
    mediator.get_current_feature();
    mediator.execute_task(RENDER_FEATURE);
    mediator.execute_task(READ_NAVIGATION_BUTTON);
    mediator.taskArgs.feature = mediator.taskResults.currentFeature;
    mediator.set_current_feature();
    mediator.taskArgs.task = mediator.taskResults.currentTask;
    mediator.set_current_task();
    //For testing, we execute task BLINK_LED and stop this task when we receive message from MQTT broker
    mediator.taskArgs.blinkLedPin = 2;
    mediator.taskArgs.task = BLINK_LED;
    mediator.set_current_task();
    mediator.set_current_task_status(false);
    if (mediator.isTaskExecutable) {
        while ((!mediator.isTaskCompleted) & (!mediator.isTaskQueueEmpty)) {
            mediator.taskArgs.task = mediator.taskResults.currentTask;
            mediator.execute_task(mediator.taskArgs.task);
            mediator.execute_task(READ_NAVIGATION_BUTTON);
            mediator.execute_task(SUBSCRIBE_MQTT_TOPIC);
            mediator.get_current_task_status();
            yield();
        }

        Serial.println(F("Task executed completely"));
////        taskArgs.taskName = IDLE;
////        mediator.execute_task(SET_TASK);
    }
}