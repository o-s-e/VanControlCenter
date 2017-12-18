// WifiInterface.h

#ifndef WIFIINTERFACE_H
#define WIFIINTERFACE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ByteBuffer.h>
#include <Configuration.h>

#include "ConsoleForm.h"
#include "HWConfig.h"
#include "Utils.h"

/**
* Class to interface to ESP8266
*/

//Logger tag
#define WIFI_TAG				F("WIFI")

//Cfg file
#define WIFI_CFG_FILE			"PHONE.CFG"

//Phone rx buffer size
#define WIFI_RX_BUFFER_SIZE	128

//Phone packets header
#define INFO_PACKET		"INF"
#define GPS_PACKET		"GPS"
#define TEMP_PACKET		"TEM"
#define LIGHT_PACKET	"LIG"

//Struct containing the infos sent by the esp8266
#pragma pack(push, 1)
using infoData = struct InfoData {
    char date[8];
    char time[8];
    byte ack;
};
#pragma pack(pop)

#pragma pack(push, 1)
using gpsData = struct GpsData {
    double latitude;
    double longitude;
    double altitude;
    double accuracy;
    double speed;
    byte ack;
};
#pragma pack(pop)

#pragma pack(push, 1)
using tempData = struct TempData {
    byte status;
    double temp;
    double hum;
    byte ack;
};
#pragma pack(pop)

#pragma pack(push, 1)
using lightData = struct LightData {
    uint8_t light1;
    uint8_t light2;
    uint8_t light3;
    uint8_t light4;
    uint8_t light5;
    uint8_t light6;
    byte ack;
};
#pragma pack(pop)

//Events handler
using gpsDataHandler = void(*)(const gpsData&);

class WifiInterfaceClass {
public:
    //Function to call in the setup
    void init();

    //Function to call in the loop
    void update();

    //Set the callback to invoke when a gps packet arrived
    void setGpsDataHandler(gpsDataHandler);

private:
    //Cfg file properties
    enum Attr : byte {
        PHONE_NUM
    };

    //Last received packets
    infoData info_;
    gpsData gps_;
    tempData temp_;
    lightData light_;

    //RX byte buffer
    ByteBuffer rxBuffer_;

    //Gps callback
    gpsDataHandler gpsHandler_;

    //Parse incoming packets
    boolean parsePacket(const char* header, byte* buffer, int size);
};

//Wifi interface instance
extern WifiInterfaceClass wifiInterface;

#endif
