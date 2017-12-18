#ifndef HEATER_INTERFACE_H
#define HEATER_INTERFACE_H

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
#include "ChannelsBuffer.h"
#include "CanInterface.h"

// Logger tag
#define HT_TAG F("Heater")

// Heater state string
#define ON_STATE "ON"
#define OFF_STATE "OFF"
#define VENT_STATE "VENT"
#define ERROR_STATE "ERROR"

#define HT_STATE_TTL 5000
#define HT_STATE_LED_DUR 10000
#define HT_COOLDOWN_DUR 30000


class HeaterInterface {
public:
    // constructor

    // Enum containing the Heater state
    enum HeaterState : byte { OFF, ON, VENT_ONLY, UNKNOWN, ERROR };

    void init();

    // Function to call in the loop
    void update();
    // function to call when we set a new state by LCD or Web
    void onStateChanged(const char* state);

    // Return the current state
    HeaterState getState() { return state_; }

    void heaterFaultCodeCallback();

private:

    void cooldown();

    uint8_t heaterFaultCode_;

    // Heater led status
    byte ledStatus_;

    // Current Temperature Controller state
    HeaterState state_;


    // TempCtrl state TTL timer
    Timer lastStateUpdate_;

    // TempCtrl state LED timer
    Timer ledTimer_;

    Timer cooldownTimer_;
};

extern HeaterInterface heaterInterface;

#endif
