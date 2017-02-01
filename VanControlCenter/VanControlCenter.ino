#include <Arduino.h>
//
//    FILE: VanCenter.c
//  AUTHOR: Olivier Van Steenberghe
// VERSION: 0.0.01
// PURPOSE: Global Van control center
//     URL:
//
// Released to the public domain
//
#include "HeaterInterface.h"

#include "CanInterface.h"
#include "Channel.h"
#include "ChannelsBuffer.h"
#include "ChannelsConfig.h"
#include "ConsoleForm.h"
#include "DataLogger.h"
#include "DisplayInterface.h"
#include "HWConfig.h"

#include "LCDForm.h"
#include "LCDStringList.h"
#include "LCDStringMsg.h"
#include "Logger.h"

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

// Code flag
#define WDT_ON
#define LCD_ON
#define DL_ON

#define LOOP_DEBUG

// Log tag
#define INIT_TAG F("INIT")
#define LOOP_TAG F("LOOP")

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

	// Update Heater
	Heater.update();

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

	Log.i(LOOP_TAG) << F("Loop calls: ") << loops << F("\t avgExecTime: ")
		<< (float)avgExecutionTime / loops * 1000 << F(" us ")
		<< F("\t freeMem: ") << freeMemory() << Endl;
	avgExecutionTime = 0;
	loops = 0;
	sec.start();
}

// Inits functions
void initPorts() {
	INIT_SERIAL(LOG_SERIAL, LOG_SERIAL_BAUD);
	INIT_SERIAL(ES_SERIAL, ES_SERIAL_BAUD);
	// Heater Initialisation
	Heater.init();

	canInterface.init(CAN_SPEED);
	canInterface.setCanEventCallBack(&onCanPacketReceived);
	// Utils
	Log.init(&LOG_SERIAL);
}

bool initDataLogger() {
	// Datalogger
	if (channelsConfig.init()) {
		// channelsConfig.debug();
		channelsBuffer.init();
		dataLogger.init();
		return true;
	}

	return false;
}

void onCanPacketReceived(CAN_FRAME &frame) {
	// Log.i(CAN_TAG) << F("Received ") << frame.id << " " << frame.length << "
	// " << Hex << Log.array<byte>(frame.data.bytes, frame.length) << Endl;

	channelsBuffer.setValue(frame.id, frame.data.bytes, frame.length);
}
