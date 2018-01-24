#ifndef HWCONFIG_H
#define HWCONFIG_H

#include "Arduino.h"

/**
* Contain all the hardware definitions
*/

#define DEBUGVAL

// loop
#define RUN_LED 6

// Heater
#define HT_GAS_PIN 24
#define HT_VENT_PIN 26
#define HT_INPUT_PIN 28
#define HT_STATE_LED 32

// ESP8266
#define WIFI_SERIAL Serial1
#define WIFI_SERIAL_BAUD 115200
#define WIFI_RST_PIN 34

// WDT
#define WDT_TIMEOUT 30000

// Log
#define LOG_SERIAL Serial
#define LOG_SERIAL_BAUD 115200

// LCD
#define LCD_SERIAL Serial2
#define LCD_SERIAL_BAUD 9600
#define LCD_RESET_PIN 36

// CAN
#define CAN_DEBUG_SERIAL SerialUSB
#define CAN_SERIAL_BAUD 115200
#define CAN_SPEED CAN_BPS_125K
//SD
#define SD_SS_PIN					23

//Light

#define RGB_RED 10
#define RGB_GREEN 11
#define RGB_BLUE 12
#define WHITE_LED 9
#define AWNING_LED 8
#define WORKTOP_LED 7

#endif
