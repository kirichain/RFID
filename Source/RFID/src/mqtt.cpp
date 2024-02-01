//
// Created by Tan on 30-Oct-23.
//

#include "mqtt.h"

AsyncMqttClient mqttClient;

MQTT *MQTT::instance = nullptr;

MQTT::MQTT() {
    instance = this;
    Serial.println(F("MQTT instance initiated"));
}

void MQTT::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println(F("Disconnected from MQTT. Try reconnecting"));
    mqttClient.connect();
}

void MQTT::onMqttConnectStatic(bool sessionPresent) {
    if (MQTT::instance) {
        MQTT::instance->onMqttConnect(sessionPresent);
    }
}

void MQTT::onMqttConnect(bool sessionPresent) const {
    Serial.println(F("Connected to MQTT."));
//    Serial.print(F("Session present: "));
//    Serial.println(sessionPresent);
    mqttClient.subscribe(lwt_topic.c_str(), 0);

    mqttClient.publish(lwt_topic.c_str(), 0, false,
                       (String(R"({"mac": ")") + String(mac_address) + String(R"(", "status": "ON"})")).c_str());
}

void MQTT::onMqttSubscribeStatic(uint16_t packetId, uint8_t qos) {
    if (MQTT::instance) {
        MQTT::instance->onMqttSubscribe(packetId, qos);
    }
}

void MQTT::onMqttSubscribe(uint16_t packetId, uint8_t qos) {
    Serial.println("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);
    Serial.print(F("Subscribed to topic: "));
    Serial.println(last_subscribed_topic);
}

void
MQTT::onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index,
                    size_t total) {
    Serial.print("Publish received: ");
    Serial.println(payload);
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  qos: ");
    Serial.println(properties.qos);
    Serial.print("  dup: ");
    Serial.println(properties.dup);
    Serial.print("  retain: ");
    Serial.println(properties.retain);
    Serial.print("  len: ");
    Serial.println(len);
    Serial.print("  index: ");
    Serial.println(index);
    Serial.print("  total: ");
    Serial.println(total);
}

bool
MQTT::connect_to_broker(const char *server_ip, int server_port, const char *_lwt_topic, const String &_mac_address) {
    lwt_topic = _lwt_topic + _mac_address;
    mac_address = _mac_address;
    lwt_payload = String(R"({"mac": ")") + mac_address + String(R"(", "status": "OFF"})");

    mqttClient.setServer(server_ip, server_port);
    mqttClient.setWill(lwt_topic.c_str(), 0, true, lwt_payload.c_str(), 0);
    mqttClient.onConnect(MQTT::onMqttConnectStatic);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onSubscribe(MQTT::onMqttSubscribeStatic);
    Serial.println(F("Connecting to MQTT broker..."));
    mqttClient.connect();
    return true;
}

bool MQTT::subscribe_topic(const char *topicName) {
    last_subscribed_topic = String(topicName);
    mqttClient.subscribe(topicName, 0);
    return true;
}

bool MQTT::publish_message(char *topicName) {
    return true;
}

void MQTT::handle_incoming_message() {
}


