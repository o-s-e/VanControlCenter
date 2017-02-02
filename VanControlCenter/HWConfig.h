#ifndef _HWCONFIG_h
#define _HWCONFIG_h

#include "arduino.h"

/**
* Contain all the hardware definitions
*/

// loop
#define RUN_LED 6

// Heater
#define HT_GAS_PIN 24
#define HT_VENT_PIN 26
#define HT_INPUT_PIN 28
#define HT_STATE_PIN 30
#define HT_STATE_LED 32

// ESP8266
#define WIFI_SERIAL Serial2
#define WIFI_SERIAL_BAUD 115200
#define WIFI_RST_PIN 34

// WDT
#define WDT_TIMEOUT 10000

// Log
#define LOG_SERIAL Serial
#define LOG_SERIAL_BAUD 115200

// LCD
#define LCD_SERIAL Serial1
#define LCD_SERIAL_BAUD 200000
#define LCD_RESET_PIN 5

// CAN
#define CAN_DEBUG_SERIAL SerialUSB
#define CAN_SERIAL_BAUD 115200
#define CAN_SPEED CAN_BPS_125K

#endif
