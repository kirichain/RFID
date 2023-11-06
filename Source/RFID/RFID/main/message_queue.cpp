//
// Created by Tan on 29-Oct-23.
//

#include "message_queue.h"

MessageQueue::MessageQueue() {

}

bool MessageQueue::publish() {
    return true;
}

bool MessageQueue::subscribe() {
    return true;
}

bool MessageQueue::en_queue() {
    return true;
}

bool MessageQueue::de_queue() {
    return true;
}

bool MessageQueue::create_topic() {
    return true;
}

int MessageQueue::get_topic_count() {
    return 0;
}

message MessageQueue::retrieve_message() {
    message _message;
    return _message;
}