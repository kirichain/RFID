//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_REQUEST_H
#define RFID_REQUEST_H

#include "Arduino.h"
#include <HTTPClient.h>

typedef struct http_response {
    byte status_code;
    String payload;
} response;

class Request {
private:
    const String get_mqtt_config_query = "?macaddress=";
    const String get_rfid_tag_info_query = "?mesKey=";
public:
    Request();

    response get(const String& query);

    response post();

    bool ping();
};
#endif //RFID_REQUEST_H
