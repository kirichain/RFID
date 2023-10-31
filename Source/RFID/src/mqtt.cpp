//
// Created by Tan on 30-Oct-23.
//

#include "mqtt.h"

MQTT::MQTT() {
    isBrokerConnected = false;
}

bool MQTT::connect_to_broker(char *serverUrl) {
    Serial.print(F("Connected to broker: "));
    Serial.println(serverUrl);
    return true;
}

message MQTT::subscribe_mqtt_topic(char *topicName) {
    message _message;
    return _message;
}

bool MQTT::publish_mqtt_message(char *topicName) {
    return true;
}