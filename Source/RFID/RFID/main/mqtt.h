//
// Created by Tan on 30-Oct-23.
//

#ifndef RFID_MQTT_H
#define RFID_MQTT_H

#include "Arduino.h"
#include "structs.h"
#include "AsyncMqttClient.h"

extern AsyncMqttClient mqttClient;

class MQTT {
    static MQTT* instance;
public:

    bool isBrokerConnected;

    const char* device_name{};
    const char* lwt_topic{};
    const char* mac_address{};

    MQTT();

    bool connect_to_broker(const char *server_ip, int server_port, const char *_lwt_topic, const char *_mac_address);

    static bool subscribe_topic(const char* topicName);

    static bool publish_message(char* topicName);

    static void onMqttConnectStatic(bool sessionPresent);

    void onMqttConnect(bool sessionPresent) const;

    static void
    onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index,
                  size_t total);

    void handle_incoming_message();

    static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
};

#endif //RFID_MQTT_H
