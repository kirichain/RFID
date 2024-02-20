//
// Created by Tan on 30-Oct-23.
//

#include "mqtt.h"

AsyncMqttClient mqttClient;

MQTT *MQTT::instance = nullptr;

MQTT::MQTT() {
    instance = this;

    is_broker_connected = false;
    is_mes_package_selected = false;

    expected_event = NONE;
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

void MQTT::onMqttConnect(bool sessionPresent) {
    Serial.println(F("Connected to MQTT."));
//    Serial.print(F("Session present: "));
//    Serial.println(sessionPresent);
    is_broker_connected = true;

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

void MQTT::onMqttMessageStatic(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len,
                               size_t index,
                               size_t total) {
    if (MQTT::instance) {
        MQTT::instance->onMqttMessage(topic, payload, properties, len, index, total);
    }
}

void
MQTT::onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index,
                    size_t total) {
//    String message(payload, len); // This will create a string with the correct length
//    // Now print message instead of payload.
//    Serial.print("Publish received: ");
//    Serial.println(message);
    Serial.println();
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

    handle_incoming_message(topic, payload, properties, len, index, total);
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
    mqttClient.onMessage(MQTT::onMqttMessageStatic);
    mqttClient.onSubscribe(MQTT::onMqttSubscribeStatic);
    Serial.println(F("Connecting to MQTT broker..."));
    mqttClient.connect();
    return true;
}

bool MQTT::subscribe_topic(const char *topicName) {
    if (mqttClient.connected()) {
        last_subscribed_topic = String(topicName);
        Serial.print(F("Subscribe to : "));
        Serial.println(topicName);
        mqttClient.subscribe(topicName, 0);
        return true;
    }
    Serial.println(F("Not connected to broker"));
    return false;
}

bool MQTT::publish_message(char *topicName) {
    return true;
}

void MQTT::handle_incoming_message(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index,
                                   size_t total) {
    String raw_last_payload = String(payload);
    for (int i = 0; i < raw_last_payload.length(); i++) {
        if (!isspace(raw_last_payload[i])) {
            last_payload += raw_last_payload[i];
        }
    }

    if (String(topic) == "rfid/mes/" + mac_address) {
        if (expected_event == MES_PACKAGE_SELECTED) is_mes_package_selected = true;
        String mes_package;

        // Find the start position of the mesKey
        int start = last_payload.indexOf(R"("mesKey":")");
        if (start != -1) {
            // Add length of "mesKey=" to start index
            start += 10;

            // Find the end of the mesKey value
            int end = last_payload.indexOf("\"", start);
            end = end == -1 ? last_payload.length() : end; // If no quote, then it's the end of the payload

            // Extract the mesKey value
            mes_package = last_payload.substring(start, end);

            Serial.print(F("Extracted MES key: "));
            Serial.println(mes_package);
        }

        last_payload = mes_package;
    }
}

void MQTT::wait_for_mqtt_event(mqtt_event_t _event){
    expected_event = _event;
    switch (_event) {
        case MES_PACKAGE_SELECTED:
            while (!is_mes_package_selected) {
                // Wait for message arrives to topic rfid/mes/{mac_address}
                //Serial.println(F("Waiting for MES package selection"));
                yield();
            }
            Serial.println(F("MES_PACKAGE_SELECTED event has arrived"));
            break;
    }
    expected_event = NONE;
}


