//
// Created by Tan on 25-Oct-23.
//

#ifndef RFID_REQUEST_H
#define RFID_REQUEST_H

#include "Arduino.h"

typedef struct response {
    byte statusCode;
    String payload;
} response;

class Request {
public:
    Request();

    response get();

    response post();
};
#endif //RFID_REQUEST_H
