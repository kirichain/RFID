//
// Created by Tan on 25-Oct-23.
//
#include "Arduino.h"
#include "mediator.h"

Mediator mediator;

void setup() {
    mediator.taskArgs.feature = BOOT;
    mediator.execute_task(SET_FEATURE);
    mediator.execute_task(RENDER_FEATURE);
    mediator.execute_task(CHECK_FS);
    mediator.execute_task(LOAD_FS);
    if ((mediator.taskResults.isFsLoaded)) {
        mediator.taskArgs.operatingMode = mediator.taskResults.savedOperatingModeInFs;
        if (mediator.taskResults.savedOperatingModeInFs == HANDHELD) {
            mediator.taskArgs.feature = HOME_HANDHELD;
        } else {
            mediator.taskArgs.feature = HOME_TERMINAL;
        }
        mediator.execute_task(SET_FEATURE);
    } else {
        mediator.taskArgs.operatingMode = HANDHELD;
    }
    mediator.execute_task(SET_OPERATING_MODE);
    mediator.execute_task(CHECK_CONNECTION);
    mediator.execute_task(INIT_AP_WIFI);
    mediator.execute_task(INIT_STA_WIFI);
}

void loop() {
    mediator.execute_task(GET_FEATURE);
    mediator.execute_task(RENDER_FEATURE);
    mediator.execute_task(READ_NAVIGATION_BUTTON);
    mediator.taskArgs.feature = mediator.taskResults.currentFeature;
    mediator.execute_task(SET_FEATURE);
    mediator.taskArgs.task = mediator.taskResults.currentTask;
    mediator.execute_task(SET_TASK);
    mediator.execute_task(SET_TASK_STATUS);

    if (mediator.isTaskExecutable) {
        mediator.execute_task(GET_TASK_STATUS);

        while (!mediator.isTaskCompleted) {
            mediator.execute_task(mediator.taskResults.currentTask);
            mediator.execute_task(READ_NAVIGATION_BUTTON);
            mediator.execute_task(SET_TASK_STATUS);
        }

        Serial.println(F("Task executed completely"));
//        taskArgs.taskName = IDLE;
//        mediator.execute_task(SET_TASK);
    }
}