//
// Created by Tan on 30-Oct-23.
//

#ifndef RFID_MQTT_H
#define RFID_MQTT_H

#include <cstdint>
#include "Arduino.h"
#include "structs.h"
#include "AsyncMqttClient.h"

extern AsyncMqttClient mqttClient;

class MQTT {
    static MQTT *instance;
private:
    const char *device_name;

    String mac_address;
    String lwt_topic;
    String lwt_payload;
    String last_subscribed_topic;
public:
    MQTT();

    static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);

    static void onMqttConnectStatic(bool sessionPresent);

    void onMqttConnect(bool sessionPresent) const;

    static void onMqttSubscribeStatic(uint16_t packetId, uint8_t qos);

    void onMqttSubscribe(uint16_t packetId, uint8_t qos);

    static void
    onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index,
                  size_t total);

    bool connect_to_broker(const char *server_ip, int server_port, const char *_lwt_topic, const String &_mac_address);

    bool subscribe_topic(const char *topicName);

    static bool publish_message(char *topicName);

    void handle_incoming_message();
};

#endif //RFID_MQTT_H
