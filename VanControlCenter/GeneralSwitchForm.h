// GeneralSwitchForm.h

#ifndef _GENERALSWITCHFORM_h
#define _GENERALSWITCHFORM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "LCDForm.h"
#include "LCDStringList.h"
#include "LCDStringMsg.h"
#include "Timer.h"
#include "DisplayInterface.h"
#include "ChannelsBuffer.h"


//Logger Tag
#define SWITCHC_TAG F("SWITCHC")

#define SWITCH_WATER 0
#define SWITCH_1 1

class GeneralSwitchForm : public LCDForm {
public:

	void init(Genie& genie);

	void update(Genie& genie);

	void onEvent(Genie& genie, genieFrame& evt);

	//TODO index needs to be adapted when the UI was build
	int getFormIndex() { return 5; }

private:

	//Update all the widgets values

	void updateWidgetsValues(Genie& genie);

	Timer lastStateChange;
};
extern GeneralSwitchForm generalSwitchForm;

#endif
