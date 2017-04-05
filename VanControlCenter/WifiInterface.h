// WifiInterface.h

#ifndef _WIFIINTERFACE_h
#define _WIFIINTERFACE_h

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
#define TEMP_PACKET		"ACC"
#define CALL_PACKET		"CALL"

//Phone commands header
#define CALL_CMD			"TEL:"

//Default phone number to call if the cfg file parse failed
#define DEFAULT_PHONE_NUM	F("+111111111")

//Struct containing the infos sent by the esp8266
#pragma pack(push, 1)
typedef struct InfoData {
	char date[8];
	char time[8];
	byte ack;
}InfoData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct GpsData {
	double latitude;
	double longitude;
	double altitude;
	double accuracy;
	double speed;
	byte ack;
}GpsData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct TempData {
	byte status;
	double temp;
	byte ack;
}TempData;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct CallData {
	boolean status;
	byte ack;
}CallData;
#pragma pack(pop)

//Events handler
typedef void(*GpsDataHandler)(const GpsData&);
//typedef void(*AccelerometerDataHandler)(AccData&);

class WifiInterfaceClass {
public:
	//Function to call in the setup
	void init();

	//Function to call in the loop
	void update();

	//Request the phone to make a phone call
	void startCall();

	//Set the callback to invoke when a gps packet arrived
	void setGpsDataHandler(GpsDataHandler);

	//Check if there is an active phone call
	boolean isCallActive() { return call.status; }

	//Start phone call button handler
	friend void onCallButtonPress(void* data);

private:
	//Cfg file properties
	enum Attr : byte {
		PHONE_NUM
	};

	//Phone number to call
	String phoneToCall;

	//Last received packets
	InfoData info;
	GpsData gps;
	TempData temp;
	CallData call;

	//RX byte buffer
	ByteBuffer rxBuffer;

	//Gps callback
	GpsDataHandler gpsHandler;

	//Parse incoming packets
	boolean parsePacket(const char* header, byte* buffer, int size);
};

//Wifi interface instance
extern WifiInterfaceClass wifiInterface;

#endif
