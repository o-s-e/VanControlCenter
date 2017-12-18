// DebugForm.h

#ifndef DEBUGFORM_H
#define DEBUGFORM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/**
* Class representing the debug form
*/

#include "CanInterface.h"
#include "ChannelsBuffer.h"
#include "DisplayInterface.h"
#include "HWConfig.h"
#include "LCDForm.h"

// Widgets index
// Can
#define CAN_ON_BLUETOOTH_BUTTON 0
#define CAN_ON_SERIAL_BUTTON 1
#define CAN_STOP_BUTTON 2
// Driver
#define DRIVER_MAP_DIGIT 7
#define DRIVER_SETTING_BUTTON 12
// BMS
#define HEATER_STATUS_STRING 3
#define CELLS_BARS 0
#define CELLS_BARS_NUM 12
#define CELLS_MAX_VOLT 4.2
#define BATTERY_V_DIGITS 6
#define BMS_DEBUG_BUTTON 14

class DebugFormClass : public LcdForm {
public:
    // LCDForm abstract method implementation
    void init(Genie& genie) override;

    // LCDForm abstract method implementation
    void update(Genie& genie) override;

    // LCDForm abstract method implementation
    // Handle the press of the touch screen
    void onEvent(Genie& genie, genieFrame& evt) override;

    // LCDForm abstract method implementation
    int getFormIndex() override { return 5; }
};

// Debug form instance
extern DebugFormClass debugForm;

#endif
