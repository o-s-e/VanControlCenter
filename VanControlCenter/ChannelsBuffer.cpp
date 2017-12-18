#include "ChannelsBuffer.h"

void ChannelsBufferClass::init() {
    //Resize the buffer
    int size = channelsConfig.getChannelCount();
    buffer_.resize(size);


    bufferSize_ = 0;
    //Create the "matrix" to contain byte-array data

    for (uint16_t i = 0; i < size; i++) {

        Channel * c = channelsConfig.getChannelByIndex(i);
        buffer_[i].resize(c->getSize());
        buffer_[i].clear();

        bufferSize_ += c->getSize();
    }
}

void ChannelsBufferClass::debug() {
    LOGLN(F("=========== Channels data buffer: ==========="));
    for (int i = 0; i < buffer_.getCapacity(); i++) {
        LOG(channelsConfig.getChannelByIndex(i)->name);
        LOG(F(": "));
        buffer_[i].debug();
    }
    LOGLN(F("========================================"));
}

String ChannelsBufferClass::getValueAsString(const unsigned short id) {
    //If CFG file was loaded
    if (channelsConfig.isValid()) {
        //Search the channel
        int index = channelsConfig.getChannelIndex(id);
        //If channel config found
        if (index != -1) {
            Channel* c = channelsConfig.getChannelByIndex(index);

            //Convert to arduino String obj
            switch (c->getDataType()) {
            case Channel::BIT_FLAG:
                return buffer_[index].toBinString(MSBFIRST);

            case Channel::DECIMAL:
                //If size <= is float
                //Need this difference because conversion is a copy 'n paste of memory
                if (c->getSize() <= 4) {
                    return String(buffer_[index].as<float>(), 6);
                }
                return String(buffer_[index].as<double>(), 10);

            case Channel::INTEGER:
                return intToString(c, buffer_[index].data());

            case Channel::U_INTEGER:
                return uintToString(c, buffer_[index].data());

            case Channel::STRING:
                return buffer_[index].toString();

            default:
                Log.e(CHBUF_TAG) << F("in getValueAsString\t Unknown conversion type channel ") << id << F(" to type ") << c->getDataType() << Endl;
            }
        }
    }
    //Error
    return F("nil");
}

ByteBuffer ChannelsBufferClass::getValueAsByteArray(unsigned short id) {
    //If CFG file was loaded
    if (channelsConfig.isValid()) {
        //Search the channel
        int index = channelsConfig.getChannelIndex(id);
        //Return a copy
        if (index != -1) {
            ByteBuffer& b = buffer_[index];
            return ByteBuffer(b.data(), b.getSize());
        }
    }
    return ByteBuffer();
}

void ChannelsBufferClass::setValue(unsigned short id, byte* data, unsigned short size) {
    //If CFG file was loaded
    if (channelsConfig.isValid()) {
        //Search the channel
        int index = channelsConfig.getChannelIndex(id);
        if (index != -1) {
            //If size received is not equal to expected size
            if (size < channelsConfig.getChannelByIndex(index)->getSize()) {
                Log.w(CHBUF_TAG) << F("In setValue\t Received size < expected size for channel ") << id << Endl;
            }
            else if (size > channelsConfig.getChannelByIndex(index)->getSize()) {
                Log.w(CHBUF_TAG) << F("In setValue\t Received size > expected size for channel ") << id << Endl;
            }

            //If size is <= than the expected size can save the value
            if (size <= channelsConfig.getChannelByIndex(index)->getSize()) {
                //Clear and save data
                buffer_[index].clear();
                buffer_[index].append(data, size);
                //Reset ttl timer
                channelsConfig.getChannelByIndex(index)->resetTtlTimer();
            }
        }
        else {
            Log.w(CHBUF_TAG) << F("ChannelsBuffer::setValue  unknow packet id = ") << id << Endl;
        }
    }
}

boolean ChannelsBufferClass::isValueUpdated(unsigned short id) {
    //If CFG file was loaded
    if (channelsConfig.isValid()) {
        //Search the channel
        int index = channelsConfig.getChannelIndex(id);
        if (index != -1) {
            //Return channel TTL timer status
            return !channelsConfig.getChannelByIndex(index)->hasTtlFinished();
        }
    }
    return false;
}

//Assuming CPU is LITTLE-ENDIAN
String ChannelsBufferClass::uintToString(Channel* channel, byte* data) {
    byte mem[8];
    char temp[21]; //64 bit unsigned integer max digit number + \0
    String value;

    for (uint64_t i = 0; i < channel->getSize(); i++) {
        mem[i] = data[i];
    }

    for (uint64_t i = channel->getSize(); i < sizeof(long long int); i++) {
        mem[i] = 0x00;
    }

    sprintf(temp, "%llu", *(reinterpret_cast<unsigned long long int*>(mem)));
    value += temp;
    return value;
}

//Assuming CPU is LITTLE-ENDIAN
String ChannelsBufferClass::intToString(Channel* channel, byte* data) {
    byte mem[8];
    char temp[21]; //64 bit signed integer max digit number + \0
    String value;

    for (uint64_t i = 0; i < channel->getSize(); i++) {
        mem[i] = data[i];
    }

    if (mem[channel->getSize() - 1] & (1 << 7)) {
        for (uint64_t i = channel->getSize(); i < sizeof(long long int); i++) {
            mem[i] = 0xFF;
        }
    }
    else {
        for (uint64_t i = channel->getSize(); i < sizeof(long long int); i++) {
            mem[i] = 0x00;
        }
    }

    sprintf(temp, "%lld", *(reinterpret_cast<long long int*>(mem)));
    value += temp;
    return value;
}

void ChannelsBufferClass::sendOnStream(UARTClass* stream) {
    unsigned short ID;
    Channel* c;
    for (uint16_t i = 0; i < channelsConfig.getChannelCount(); i++) {
        //Get the i-nth channel
        c = channelsConfig.getChannelByIndex(i);
        ID = c->getID();
        //Send the can id
        stream->write(reinterpret_cast<byte*>(&ID), sizeof(ID));
        //Send the data
        stream->write(buffer_[i].data(), buffer_[i].getCapacity());
    }
}

ChannelsBufferClass channelsBuffer;