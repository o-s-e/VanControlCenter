//
//    FILE: VanCenter.c
//  AUTHOR: Olivier Van Steenberghe
// VERSION: 0.0.01
// PURPOSE: Global Van control center
//     URL:
//
// Released to the public domain
//


#include "GeneralSwitchForm.h"
#include "LightControllerForm.h"
#include "TemperatureControllerForm.h"
#include "ConsoleForm.h"
#include "GeneralSwitchForm.h"
#include "LCDForm.h"
#include "LCDStringList.h"
#include "LCDStringMsg.h"
#include "HeaterInterface.h"
#include "LightInterface.h"
#include "CanInterface.h"
#include "WifiInterface.h"
#include "DisplayInterface.h"
#include "Channel.h"
#include "ChannelsBuffer.h"
#include "ChannelsConfig.h"
#include "DataLogger.h"
#include "HWConfig.h"
#include "Shell.h"
#include "Logger.h"

#include <Arduino.h>
#include <BitArray.h>
#include <Button.h>
#include <ByteBuffer.h>
#include <Configuration.h>
#include <DueTimer.h>
#include <SD.h>
#include <SPI.h>
#include <Timer.h>
#include <Utils.h>
#include <Vector.h>
#include <due_can.h>
#include <genieArduino.h>

// TempCtrl interface instance

void initPorts();
bool initDataLogger();
void onCanPacketReceived(CAN_FRAME &frame);
//void heaterCallback();

// Code flag
#define WDT_ON
#define LCD_ON
#define DL_ON
#define WIFI_ON
#define SHELL_ON
#define LOOP_DEBUG

// Log tag
#define INIT_TAG F("INIT")
#define LOOP_TAG F("LOOP")
#define WIFI_TAG F("WIFI")
// SW info
#define SW_REV F("1")
#define SW_INFO                                                                \
  String(F("VanControlCenter SW Rev ")) + SW_REV + String(F(" built ")) +      \
      F(__DATE__) + String(" ") + F(__TIME__)
///////////////////////////

// Test stuff
boolean ledStatus;
int loops;
unsigned long avgExecutionTime;
Timer t, sec;

#ifdef WDT_ON
void watchdogSetup() {}
#endif

void setup() {
	// Serial, digital pin, Heater, WLan, Web, Sensors
	initPorts();

	// Start timer to count init time
	t.start();

#ifdef LCD_ON
	displayInterface.init();
	consoleForm.println(SW_INFO);
	Log.i(INIT_TAG) << SW_INFO << Endl;
#endif

#ifdef DL_ON
	if (initDataLogger()) {
		consoleForm.println(F("Datalogger OK"));
		Log.i(DL_TAG) << F("Datalogger OK ") << t.elapsedTime() << Endl;
	}
	else {
		consoleForm.println(F("Datalogger FAIL"));
		Log.e(DL_TAG) << F("Datalogger FAIL") << Endl;
	}
#endif

	//Wifi
#ifdef WIFI_ON
	wifiInterface.init();
	consoleForm.println(F("Wifi OK"));
	Log.i(WIFI_TAG) << F("Wifi OK ") << t.elapsedTime() << Endl;
#endif

	// Notify init completed
	Log.i(INIT_TAG) << F("VanControlCenter init OK ") << t.elapsedTime() << Endl;

	// Init time
	Log.i(INIT_TAG) << F("Time needed to init: ") << t.elapsedTime() << Endl;

	// Init test stuff
	ledStatus = LOW;
	sec.setDuration(1000).start();
	avgExecutionTime = 0;
	loops = 0;
	// Enable WDT
#ifdef WDT_ON
	watchdogEnable(WDT_TIMEOUT);
#else
	watchdogDisable();
#endif
}

void loop() {
	// Reset Watchdog
#ifdef WDT_ON
	watchdogReset();
#endif

	// Test execution time
	t.start();

	// Read all packets and update debug if necessary
	canInterface.update();

#ifdef SHELL_ON
    //Shell update
    shell.update();
#endif

	// Update Heater
	heaterInterface.update();

	//Update LightInterface

	lightInterface.update();

#ifdef DL_ON
	// Log data on SD
	dataLogger.update();
#endif

#ifdef LCD_ON
	// Display update
	displayInterface.update();
#endif



	// loop
	// Test execution time
	avgExecutionTime += t.elapsedTime();
	loops++;

	if (sec.hasFinished()) {
		ledStatus = !ledStatus;
		digitalWrite(RUN_LED, ledStatus);
	}

	//Log.i(LOOP_TAG) << F("Loop calls: ") << loops << F("\t avgExecTime: ")
	//	<< (float)avgExecutionTime / loops * 1000 << F(" us ")
	//	<< F("\t freeMem: ") << freeMemory() << Endl;
        
	avgExecutionTime = 0;
	loops = 0;
	sec.start();
}

// Inits functions
void initPorts() {
	INIT_SERIAL(LOG_SERIAL, LOG_SERIAL_BAUD);
	INIT_SERIAL(WIFI_SERIAL, WIFI_SERIAL_BAUD);
	canInterface.setCanDebugSerial(&CAN_DEBUG_SERIAL);


	INIT_SD(SD, SD_SS_PIN);
	// Utils
	Log.init(&LOG_SERIAL);

	//Canbus Initialisation
	canInterface.init(CAN_SPEED);
	canInterface.setCanEventCallBack(&onCanPacketReceived);

	// Heater Initialisation
	heaterInterface.init();

	//Lights Initialisation

	lightInterface.init();

	//Heater input pin interrupt Initialisation
	//attachInterrupt(digitalPinToInterrupt(HT_INPUT_PIN), heaterCallback, HIGH);

#ifdef SHELL_ON
	shell.init(&LOG_SERIAL);
#endif
}

bool initDataLogger() {
	// Datalogger
	if (channelsConfig.init()) {
		//Channeldebug enabled
		channelsConfig.debug();
		channelsBuffer.init();
		dataLogger.init();
		return true;
	}

	return false;
}


//Callback to count the High on the heater led pin to get the fault codes
//void heaterCallback() {
//	heaterInterface.heaterFaultCodeCallback();
//}

void onCanPacketReceived(CAN_FRAME &frame) {
	Log.i(CAN_TAG) << F("Received ") << frame.id << " " << frame.length << " " << Hex << Log.array<byte>(frame.data.bytes, frame.length) << Endl;

	channelsBuffer.setValue(frame.id, frame.data.bytes, frame.length);

	switch (frame.id) {
	}
}