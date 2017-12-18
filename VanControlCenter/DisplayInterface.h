// DisplayInterface.h

#ifndef _DISPLAYINTERFACE_h
#define _DISPLAYINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#include "logger.h"
#endif

/**
* This class represent a generic interface to handle different form on the lcd
* screen
*/

#include <Timer.h>
#include <Vector.h>
#include <genieArduino.h>

#include "ConsoleForm.h"
#include "DebugForm.h"
#include "TemperatureControllerForm.h"
#include "LightControllerForm.h"
#include "GeneralSwitchForm.h"
#include "HWConfig.h"
#include "LCDForm.h"

// Logger tag
#define LCD_TAG F("LCD")

// Refresh per second
#define REFRESH_RATE 3

// Max time allowed to refresh a form
#define MAX_UPDATE_TIME 1500

#define CONSOLEFORM 0
#define LIGHTCTRLFORM 1
#define TEMPCTRLFORM 2
#define GENERALFORM 3
#define DEBUGFORM 4


class DisplayInterfaceClass {

public:

    // Init function to call inside the setup
    void init();

    // Update function to call inside the loop
    void update();

    // Change the current form
    void setCurrentForm(LcdForm* currentForm);

    // Return the current form
    const LcdForm* getCurrentForm() { return currentForm_; }

    // Function to handle the incoming event from the lcd
    friend void onEvent();

private:

    // Indicates if the form refresh is enabled
    boolean enabled_;

    // Genie library instance
    Genie genie_;

    // Refresh form timer
    Timer refreshTimer_;

    // Current form instance pointer
    LcdForm* currentForm_;

};

// Display interface instance
extern DisplayInterfaceClass displayInterface;

#endif
