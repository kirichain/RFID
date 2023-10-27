//
// Created by Tan on 25-Oct-23.
//
#include "Arduino.h"
#include "mediator.h"

Mediator mediator;

void setup() {
    mediator.execute_task(CHECK_FS);
    mediator.execute_task(LOAD_FS);
    mediator.execute_task(GET_OPERATING_MODE);
    mediator.execute_task(CHECK_CONNECTION);
    mediator.execute_task(INIT_AP_WIFI);
    mediator.execute_task(INIT_STA_WIFI);
}

void loop() {
    mediator.execute_task(GET_OPERATING_MODE);
    mediator.execute_task(RENDER_OPERATING_MODE);
    mediator.execute_task(GET_FEATURE);
    mediator.execute_task(RENDER_FEATURE);
    mediator.execute_task(GET_NAVIGATION);
    mediator.execute_task(SET_FEATURE);
    mediator.execute_task(SET_FEATURE_TASK);
    mediator.execute_task(SET_FEATURE_TASK_STATUS);
    mediator.execute_task(GET_FEATURE_TASK_STATUS);

    if (isTaskExecutable) {
        while (!isTaskCompleted) {
            mediator.execute_task(currentTask);
            mediator.execute_task(GET_NAVIGATION);
            mediator.execute_task(SET_FEATURE_TASK_STATUS);
        }
    }
}