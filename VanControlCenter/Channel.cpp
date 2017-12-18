#include "Channel.h"

Channel::Channel() {
    ttlTimer_.setDuration(CH_BUFFER_TTL).start();
    size_ = 0;
    id_ = 0;
    type_ = INTEGER;
}

void Channel::setSize(byte size) {
    this->size_ = size;
}

void Channel::setId(unsigned short id) {
    this->id_ = id;
}

void Channel::setDataType(DataTypes type) {
    this->type_ = type;
}

void Channel::setName(String name) {
    this->name = name;
}

byte Channel::getSize() {
    return this->size_;
}

unsigned short Channel::getID() {
    return this->id_;
}

Channel::DataTypes Channel::getDataType() {
    return this->type_;
}

String Channel::getName() {
    return this->name;
}

void Channel::resetTtlTimer() {
    ttlTimer_.start();
}

bool Channel::hasTtlFinished() {
    return ttlTimer_.hasFinished();
}
