// Logger.h

#ifndef LOGGER_H
#define LOGGER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ByteBuffer.h>

#define ASSERT_TAG F("ASSERT")

enum LogManip : char {
    Endl,

    // Log type
    Info = 0x11,
    Warning = 0x12,
    Error = 0x13,

    // Log mode
    Bin = BIN,
    Oct = OCT,
    Dec = DEC,
    Hex = HEX
};

class LoggerClass {
public:
    void init(UARTClass* serialPort);

    friend LoggerClass& operator<<(LoggerClass& log, byte value);
    friend LoggerClass& operator<<(LoggerClass& log, char value);
    friend LoggerClass& operator<<(LoggerClass& log, int value);
    friend LoggerClass& operator<<(LoggerClass& log, unsigned int value);
    friend LoggerClass& operator<<(LoggerClass& log, long value);
    friend LoggerClass& operator<<(LoggerClass& log, unsigned long value);

    friend LoggerClass& operator<<(LoggerClass& log, float value);
    friend LoggerClass& operator<<(LoggerClass& log, double value);

    friend LoggerClass& operator<<(LoggerClass& log, const char* value);
    friend LoggerClass& operator<<(LoggerClass& log, const String& value);
    friend LoggerClass& operator<<(LoggerClass& log,
                                   const __FlashStringHelper* value);

    friend LoggerClass& operator<<(LoggerClass& log, LogManip value);
    friend LoggerClass& operator<<(LoggerClass& log, LoggerClass& value);

    friend LoggerClass& operator<<(LoggerClass& log, ByteBuffer& value);

    LoggerClass& i(const char* tag = "INFO");
    LoggerClass& w(const char* tag = "WARN");
    LoggerClass& e(const char* tag = "ERR");

    LoggerClass& i(const __FlashStringHelper* tag);
    LoggerClass& w(const __FlashStringHelper* tag);
    LoggerClass& e(const __FlashStringHelper* tag);

    void setDecimalPrecision(int precision);

    template <typename T>
    LoggerClass& array(T* a, int size) {
        for (int i = 0; i < size; i++) {
            (*this) << a[i] << " ";
        }
        return (*this);
    }

    LoggerClass& repeat(char c, int times);

    void assert(boolean condition, const String& msg);

private:
    byte precision_ = 0;
    UARTClass* serialPort_ = nullptr;
    LogManip mode_ = Info;
};

extern LoggerClass Log;

#endif
