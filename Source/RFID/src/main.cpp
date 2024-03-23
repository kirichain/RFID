//
// Created by Tan on 25-Oct-23.
//
#include "Arduino.h"
#include "mediator.h"

Mediator mediator;

// Serial: For debugging
// Serial 1: For communicating with rfid module
// Serial 2: For communicating with computer
void setup() {
    Serial.begin(115200);
    Serial2.begin(115200);
    mediator.init_services();

    mediator.execute_task(INIT_STA_WIFI);
    mediator.taskArgs.feature = HOME_HANDHELD_2;
    mediator.execute_task(RENDER_FEATURE);
    mediator.set_current_feature();
}

void loop() {
    mediator.get_current_feature();
    mediator.execute_task(RENDER_FEATURE);
    mediator.set_current_feature();
    mediator.execute_task(READ_NAVIGATION_BUTTON);
    mediator.execute_task(READ_SERIAL_COMMUNICATION_MESSAGE);
    mediator.execute_task(HANDLE_MQTT_MESSAGE);
    mediator.execute_task(CHECK_WIFI_CONNECTION);

    if ((mediator.isTaskExecutable) && (mediator.taskArgs.task != NO_TASK) && (mediator.taskArgs.task != IDLE)) {
        Serial.println(F("Task execution starts"));
        while ((!mediator.isTaskCompleted)) {
            mediator.taskArgs.task = mediator.taskResults.currentTask;
            mediator.execute_task(mediator.taskArgs.task);
            mediator.execute_task(READ_NAVIGATION_BUTTON);
//            mediator.execute_task(READ_SERIAL_COMMUNICATION_MESSAGE);
//            mediator.execute_task(HANDLE_MQTT_MESSAGE);
//            mediator.get_current_task_status();
            yield();
        }

        Serial.println(F("Task execution completed"));
        mediator.taskArgs.task = IDLE;
        mediator.set_current_task();
    }
}