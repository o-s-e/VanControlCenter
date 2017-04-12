#ifndef HeaterInterface_h
#define HeaterInterface_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Timer.h>
/**
* Class containing the external interface to the heater relaisState
*/

#include "HWConfig.h"
#include "Logger.h"

// Logger tag
#define HT_TAG F("Heater")

// Heater state string
#define ON_STATE "ON"
#define OFF_STATE "OFF"
#define VENT_STATE "VENT"
#define ERROR_STATE "ERROR"

#define HT_STATE_TTL 50000
#define HT_STATE_LED_DUR 1000
#define HT_COOLDOWN_DUR 300000

class HeaterInterface {
public:
	// constructor

	// Enum containing the Heater state
	enum HeaterState : byte { On, Off, VentOnly, Unknown, Error };

	void init();

	// Function to call in the loop
	void update();
	// function to call when we set a new state by LCD or Web
	void onStateChanged(const char *state);

	// Return the current state
	HeaterState getState() { return state; }

private:

	void cooldown();


	// Heater led status
	byte ledStatus;

	// Current Temperature Controller state
	HeaterState state;

	// TempCtrl state TTL timer
	Timer lastStateUpdate;

	// TempCtrl state LED timer
	Timer ledTimer;

	Timer cooldownTimer;
};

extern HeaterInterface heaterInterface;

#endif
