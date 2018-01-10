#include "logger.h"

void LoggerClass::init(UARTClass* serialPort) {
    this->serialPort_ = serialPort;
    this->mode_ = Dec;
    this->precision_ = 6;
}

void LoggerClass::setDecimalPrecision(int precision) {
    this->precision_ = precision;
}

void LoggerClass::assert(boolean condition, String msg) {
    if (!condition) {
        this->e(ASSERT_TAG) << msg << Endl;
        serialPort_->flush();
        while (true);
    }
}

LoggerClass& LoggerClass::i(const char* tag) {
    (*this) << Info << tag << " ";
    return (*this);
}

LoggerClass& LoggerClass::w(const char* tag) {
    (*this) << Warning << tag << " ";
    return (*this);
}

LoggerClass& LoggerClass::e(const char* tag) {
    (*this) << Error << tag << " ";
    return (*this);
}

LoggerClass& LoggerClass::i(const __FlashStringHelper* tag) {
    (*this) << Info << tag << " ";
    return (*this);
}

LoggerClass& LoggerClass::w(const __FlashStringHelper* tag) {
    (*this) << Warning << tag << " ";
    return (*this);
}

LoggerClass& LoggerClass::e(const __FlashStringHelper* tag) {
    (*this) << Error << tag << " ";
    return (*this);
}

LoggerClass& LoggerClass::repeat(char c, int times) {
    for (int i = 0; i < times; i++) {
        Log << c;
    }
    return (*this);
}

// Print
LoggerClass& operator<<(LoggerClass& log, byte value) {
    log.serialPort_->print(value, log.mode_);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, char value) {
    log.serialPort_->print(value);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, int value) {
    log.serialPort_->print(value, log.mode_);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, unsigned int value) {
    log.serialPort_->print(value, log.mode_);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, long value) {
    log.serialPort_->print(value, log.mode_);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, unsigned long value) {
    log.serialPort_->print(value, log.mode_);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, float value) {
    log.serialPort_->print(value, log.precision_);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, double value) {
    log.serialPort_->print(value, log.precision_);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, const char* value) {
    log.serialPort_->print(value);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, const String& value) {
    log.serialPort_->print(value);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, const __FlashStringHelper* value) {
    log.serialPort_->print(value);
    return log;
}

LoggerClass& operator<<(LoggerClass& log, LogManip value) {
    switch (value) {
    case Endl:
        log.mode_ = Dec;
        log.serialPort_->println();
        log.serialPort_->flush();
        break;

    case Bin:
    case Oct:
    case Dec:
    case Hex:
        log.mode_ = value;
        break;

    default:
        log.serialPort_->print(static_cast<char>(value));
        break;
    }
    return log;
}

LoggerClass& operator<<(LoggerClass& log, LoggerClass& value) { return log; }

LoggerClass& operator<<(LoggerClass& log, ByteBuffer& b) {
    LogManip prevLogMode = log.mode_;

    Log << Hex << Log.array<byte>(b.data(), b.getSize()) << prevLogMode;
    return log;
}

LoggerClass Log;
