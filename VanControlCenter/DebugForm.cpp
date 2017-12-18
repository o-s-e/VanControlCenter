#include "DebugForm.h"

void DebugFormClass::init(Genie& genie) {
    // Nothing to do here, for now
}

void DebugFormClass::update(Genie& genie) {
    if (channelsBuffer.isValueUpdated(CanId::HEATER_STATUS)) {
        updateString(genie, HEATER_STATUS_STRING,
                     channelsBuffer.getValueAsString(CanId::HEATER_STATUS));
    }
    else {
        updateString(genie, HEATER_STATUS_STRING, F("---"));
    }
}

void DebugFormClass::onEvent(Genie& genie, genieFrame& evt) {
    //Handles the touch screen button press
    switch (evt.reportObject.index) {
        //Enable can analyzer via serial port
    case CAN_ON_SERIAL_BUTTON:
        INIT_SERIAL(CAN_DEBUG_SERIAL, CAN_SERIAL_BAUD);
        canInterface.setCanDebugSerial(&CAN_DEBUG_SERIAL);
        break;

        //Enable can analyzer via bluetooth
    case CAN_ON_BLUETOOTH_BUTTON:
        INIT_SERIAL(WIFI_SERIAL, CAN_SERIAL_BAUD);
        canInterface.setCanDebugSerial(&WIFI_SERIAL);
        //  telemetryInterface.setLogSerial(NULL);
        break;
    default:
        ;
    }
}

DebugFormClass debugForm;
