// Channel.h

#ifndef CHANNEL_H
#define CHANNEL_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Timer.h>

/**
* Class representing all the information relative to a single CAN packet
*/

// TTL timer duration
#define CH_BUFFER_TTL 5000

class Channel {
public:
    // Data type enum
    enum DataTypes : char {
        BIT_FLAG = 'B',
        U_INTEGER = 'U',
        INTEGER = 'I',
        DECIMAL = 'D',
        STRING = 'S',
        BOOL = 'O'
    };

    // CFG file attributes
    enum Attr : byte { CAN_ID, NAME, SIZE, TYPE, ATTR_COUNT };

    Channel();

    // Setters
    void setSize(byte size);
    void setId(unsigned short id);
    void setDataType(DataTypes type);
    void setName(String name);

    // Getters
    byte getSize();
    unsigned short getID();
    DataTypes getDataType();
    String getName();

    // Reset TTL timer
    void resetTtlTimer();
    // Check if the TTL timer has finished
    bool hasTtlFinished();

    // Associated channel name

    String name;

private:
    // CAN packet size
    byte size_;

    // CAN packet ID
    unsigned short id_;

    // CAN packet data type
    DataTypes type_;

    // TTL timer (time-to-live), if has not finished the last channel value is
    // still valid
    Timer ttlTimer_;
};

#endif
