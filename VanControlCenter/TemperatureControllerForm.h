// TemperatureControllerForm.h

#ifndef TEMERATURECONTROLLERFORM_H
#define TEMERATURECONTROLLERFORM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "LCDForm.h"
#include "HeaterInterface.h"
#include "LCDStringList.h"
#include "LCDStringMsg.h"
#include "Timer.h"
#include "DisplayInterface.h"
#include "ChannelsBuffer.h"

/**
* Class to control the temperature and sets the state of the HeaterInterface,
* according to the values from can and user input over the diplayController
*/

//Logger Tag
#define TEMPC_TAG F("TEMPC")

#define TEMP_GAUGE_DISP 0
#define TEMP_GAUGE_SET 1

class TemperatureControllerForm : public LcdForm {
public:

    void init(Genie& genie) override;

    void update(Genie& genie) override;

    void onEvent(Genie& genie, genieFrame& evt) override;

    //TODO index needs to be adapted when the UI was build
    int getFormIndex() override { return 3; }

private:

    //Update all the widgets values

    void updateWidgetsValues(Genie& genie);

    Timer lastStateChange_;
};

extern TemperatureControllerForm temperatureControllerForm;

#endif
