// TemperatureControllerForm.h

#ifndef _TEMERATURECONTROLLERFORM_h
#define _TEMERATURECONTROLLERFORM_h

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


class TemperatureControllerForm : public LCDForm{

public:

	void init(Genie& genie);

	void update(Genie& genie);

	void onEvent(Genie& genie, genieFrame& evt);

	//TODO index needs to be adapted when the UI was build
	int getFormIndex() { return 3; }
	
private:

	//Update all the widgets values
	void updateWidgetsValues(Genie& genie);

	Timer lastStateChange;

	float setTemp;


};

extern TemperatureControllerForm temperatureControllerForm;

#endif