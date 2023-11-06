//
// Created by Tan on 30-Oct-23.
//

#ifndef RFID_QUEUE_H
#define RFID_QUEUE_H

#include "Arduino.h"

class Queue {
public:
    Queue();

    bool en_queue();

    bool de_queue();

    bool is_queue_full();

    int get_queue_count();
};

#endif //RFID_QUEUE_H
