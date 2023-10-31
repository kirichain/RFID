//
// Created by Tan on 29-Oct-23.
//

#ifndef RFID_MESSAGE_QUEUE_H
#define RFID_MESSAGE_QUEUE_H

#include "Arduino.h"
#include "queue.h"
#include "cppQueue.h"
#include "structs.h"

class MessageQueue {
public:
    MessageQueue();

    bool publish();

    bool subscribe();

    bool en_queue();

    bool de_queue();

    bool create_topic();

    int get_topic_count();

    message retrieve_message();
};
#endif //RFID_MESSAGE_QUEUE_H
