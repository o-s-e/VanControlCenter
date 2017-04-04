// CanInterface.h

#ifndef _CANINTERFACE_h
#define _CANINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/**
* Class containing the external interface to the Can bus
*/

#include <due_can.h>
#include <Timer.h>

#include "ChannelsBuffer.h"
#include "HWConfig.h"
#include "Utils.h"

//Class containing an enum with all the CanID constants hard-coded
class CanID {
public:
	enum IDs : unsigned short {
		//BMS
		BMS_CMD = 0x40,			//Heater Control

		//Wifi
		DATE = 0x60,			//Date dd/mm/yy
		TIME = 0x61,			//Time hh:MM:ss
		GPS_LATITUDE = 0x62,	//Latitude
		GPS_LONGITUDE = 0x63,	//Longitude
		GPS_ALTITUDE = 0x64,	//Altitude
		GPS_ACCURACY = 0x65,	//Accuracy radius
		GPS_SPEED = 0x66,		//GPS valid flag

		CALL_STATUS = 0x70,		//Voice call status		0 - no call		1 - in call
		TEMP = 0x71,			//Temperature Sensor Temp
		HUM = 0x72,			//Temperature Sensor Humidity
		HEATER_STATUS = 0x73, // Heater state

	};
};

//Logget tag
#define CAN_TAG		F("CAN")

//#define CAN_SERIAL_TIMEOUT		5000

//Can analyzer send packet header
#define CAD_MSG_HEADER			"CAD"	//CAN Analyzer Data

//Can analyzer received packet header
#define CAN_MSG_HEADER			"CAN"	//SEND + CHANNEL + SIZE + DATA

//RX buffer size for incoming can analyzer packets
#define DEBUG_RX_BUFFER_SIZE	32

//Can stream
//Can stream packet delay
#define PACKET_DELAY	50

//Can stream wait for a response timeout
#define CMD_TIMEOUT		3000

//Can stream error msg
#define ERROR_CMD		"ERR"

//Can stream successful msg
#define OK_CMD			"OK"

//Enum containing the possible results of a can stream
enum CanStreamResult {
	SUCCES,
	ERROR,
	TIMEOUT,
	WRONG_ACK,
	ABORT
};

//Received can packet callback typedef
typedef void(*CanEventHandler)(CAN_FRAME&);


class CanInterfaceClass {

public:
	//Function to call in the setup
	void init(int canSpeed, unsigned short minID = 0, unsigned short maxID = 2048);

	//Fucntion to call in the loop
	void update();

	//Set the function to call when a new packet is received
	void setCanEventCallBack(CanEventHandler);

	//Remove the callback
	void removeCanEventCallBack();

	//Return the number of packets to read
	int available();

	//Return the next received packet
	CAN_FRAME& read();

	//Send a packet through the can bus
	void send(CanID::IDs id, byte* data, byte size);

	//Stream
	//Stream a byte array over can
	CanStreamResult streamOverCan(CanID::IDs canID, const char* openStreamCmd, byte* buffer, int size);

	//Wait for a byte array stream over can
	CanStreamResult waitForStreamOverCan(CanID::IDs canID, const char* openStreamCmd, byte* buffer, int expectedBytes);

	//Set the debug serial port for the can analyzer
	void setCanDebugSerial(Stream* debugSerial);


private:
	//Debug serial pointer
	Stream* debugSerial;

	//Can packet
	CAN_FRAME frame;

	//Can analyzer receive buffer
	ByteBuffer rxBuffer;

	//Packet received callback
	CanEventHandler canEvent;

	//Debug
	//Send a packet to the can analyzer
	void writePacketOnDebugSerial(CAN_FRAME& packet);

	//Read all the incoming packets from the can analyzer
	void readFromDebugSerial();

	//Parse the incoming packets from the can analyzer
	void parseSerialDebugCmd();
};

//Can bus interface instance
extern CanInterfaceClass canInterface;


#endif

