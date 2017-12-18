#include "CanInterface.h"

void CanInterfaceClass::init(int canSpeed, unsigned short minId, unsigned short maxId) {
    Can0.begin(canSpeed);
    Can0.watchForRange(minId, maxId);

    debugSerial_ = NULL;
    canEvent_ = NULL;
    rxBuffer_.resize(DEBUG_RX_BUFFER_SIZE);
    Log.i(CAN_TAG) << F("Can0 interface initialized") << Endl;
}

void CanInterfaceClass::update() {
    //If a frame is available
    if (Can0.available()) {
        Log.i(CAN_TAG) << F("Can0 available") << Endl;
        //Read
        Can0.read(frame_);

        //If the callback is set
        if (canEvent_ != NULL) {
            //Invoke the callback
            Log.i(CAN_TAG) << F("Can0 callback is set") << Endl;

            canEvent_(frame_);
        }

        //Send the packet to the can analyzer
        writePacketOnDebugSerial(frame_);
    }

    //If the can analyzer has sent packet, then read and parse
    if (debugSerial_ != NULL) {
        readFromDebugSerial();
        parseSerialDebugCmd();
    }
}

//R/W
int CanInterfaceClass::available() {
    return Can0.available();
}

CAN_FRAME& CanInterfaceClass::read() {
    //Return next available can frame
    if (Can0.available()) {
        Can0.read(frame_);

        //Echo
        writePacketOnDebugSerial(frame_);
    }
    else {
        //Invalid frame if nothing available
        frame_.id = 0;
        frame_.length = 0;
    }
    return frame_;
}

void CanInterfaceClass::send(CanId::IDs id, byte* data, byte size) {
    //Send frame over can
    frame_.id = id;
    frame_.length = size;
    memcpy(frame_.data.bytes, data, size);
    Can0.sendFrame(frame_);

    //Echo
    writePacketOnDebugSerial(frame_);
}

//Event
void CanInterfaceClass::setCanEventCallBack(canEventHandler cb) {
    this->canEvent_ = cb;
}

void CanInterfaceClass::removeCanEventCallBack() {
    this->canEvent_ = NULL;
}

//Debug
void CanInterfaceClass::setCanDebugSerial(Stream* debugSerial) {
    //Attach interrupt for Can Debug
    this->debugSerial_ = debugSerial;
}

void CanInterfaceClass::writePacketOnDebugSerial(CAN_FRAME& packet) {
    if (debugSerial_ != NULL) {
        debugSerial_->print(CAD_MSG_HEADER);
        debugSerial_->write(packet.id >> 8);
        debugSerial_->write(packet.id);
        debugSerial_->write(packet.length);
        debugSerial_->write(static_cast<byte*>(packet.data.bytes), packet.length);
    }
}

void CanInterfaceClass::readFromDebugSerial() {
    //Read from debug Serial
    if (debugSerial_->available()) {
        while (debugSerial_->available() && rxBuffer_.getSize() < rxBuffer_.getCapacity()) {
            rxBuffer_.append(debugSerial_->read());
        }
    }
}

void CanInterfaceClass::parseSerialDebugCmd() {
    //Parse debug command
    int index = rxBuffer_.indexOf(CAN_MSG_HEADER);

    //CMD Packet ->  CMD + Size + Command + Data	Size = sizeof(Command) + sizeof(data)
    if (index != -1) {
        index += sizeof(CAN_MSG_HEADER) - 1;

        if (rxBuffer_.getSize() > index + 3) {
            unsigned short id = ((rxBuffer_[index] & 0xFF) << 8) | (rxBuffer_[index + 1] & 0xFF);
            unsigned short size = rxBuffer_[index + 2] & 0xFF;
            index += 3;

            if (rxBuffer_.getSize() >= index + size) {
                send(CanId::IDs(id), &rxBuffer_[index], size);

                //test
                frame_.id = id;
                frame_.length = size;
                memcpy(frame_.data.bytes, &rxBuffer_[index], size);
                canEvent_(frame_);
                //////////////

                rxBuffer_.remove(0, index + size);
            }
        }
    }
}


CanStreamResult CanInterfaceClass::streamOverCan(CanId::IDs canId, const char* openStreamCmd, byte* buffer, int size) {
    Timer timeOut;

    //Open the stream over can
    canInterface.send(canId, (byte*)openStreamCmd, strlen(openStreamCmd));

    //Needed packets to send
    int q = size / 8;
    int r = size % 8;
    byte ack = getAck(buffer, size);

    LOGLN(size);
    LOG_ARR(buffer, size, HEX);

    //Send packets
    for (int i = 0; i < q; i++) {
        canInterface.send(canId, buffer + i * 8, 8);
        delay(PACKET_DELAY);
    }

    //Send remaining bytes
    if (r != 0) {
        canInterface.send(canId, buffer + q * 8, r);
        delay(PACKET_DELAY);
    }

    //Send ack
    canInterface.send(canId, &ack, 1);

    //Init timeout timer
    timeOut.setDuration(CMD_TIMEOUT).start();

    while (!timeOut.hasFinished()) {
        //Read all response frame
        CAN_FRAME frame = canInterface.read();
        //If the packet has the stream id
        if (frame.id == canId) {
            //if response is OK
            if (strcmp(reinterpret_cast<const char*>(frame.data.bytes), OK_CMD) == 0) {
                return SUCCESS;
            }
                //if response is ERROR
            else if (strcmp(reinterpret_cast<const char*>(frame.data.bytes), ERROR_CMD) == 0) {
                return ERROR;
            }
        }
    }
    return TIMEOUT;
}

CanStreamResult CanInterfaceClass::waitForStreamOverCan(CanId::IDs canId, const char* openStreamCmd, byte* buffer,
                                                        int expectedBytes) {
    Timer timeOut;


    //Open the stream over can
    canInterface.send(canId, (byte*)openStreamCmd, strlen(openStreamCmd));

    //Set timeout timer
    timeOut.setDuration(CMD_TIMEOUT).start();

    int memIndex = 0;
    //Wait for can packet
    while (!timeOut.hasFinished() && memIndex < expectedBytes) {
        CAN_FRAME frame = canInterface.read();
        //Read responses
        if (frame.id == canId) {
            //On error
            if (strcmp(reinterpret_cast<const char*>(frame.data.bytes), ERROR_CMD) == 0) {
                return ERROR;
            }
            //Save data in the buffer
            memcpy(
                buffer + memIndex,
                frame.data.bytes,
                memIndex + frame.length > expectedBytes ? expectedBytes - memIndex : frame.length
                //Buffer over-run check
            );
            memIndex += frame.length;
        }
    }

    //Timeout
    if (memIndex < expectedBytes) {
        return TIMEOUT;
    }
    //Wait for ack
    while (!timeOut.hasFinished()) {
        frame_ = canInterface.read();
        if (frame_.id == canId && frame_.length == 1) {
            byte ack = frame_.data.bytes[0];
            if (ack != getAck(buffer, expectedBytes)) {
                //LOGLN((int)ack);
                //LOGLN((int)getAck(buffer, expectedBytes))
                return WRONG_ACK;
            }
            return SUCCESS;
        }
    }
    return TIMEOUT;
}

CanInterfaceClass canInterface;
