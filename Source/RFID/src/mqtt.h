//
// Created by Tan on 30-Oct-23.
//

#ifndef RFID_MQTT_H
#define RFID_MQTT_H

#include "Arduino.h"
#include "structs.h"

class MQTT {
public:
    bool isBrokerConnected;
    
    MQTT();

    bool connect_to_broker(char* serverUrl);

    message subscribe_mqtt_topic(char* topicName);

    bool publish_mqtt_message(char* topicName);

};

#endif //RFID_MQTT_H
