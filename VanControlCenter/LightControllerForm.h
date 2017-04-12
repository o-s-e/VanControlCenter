// LightControllerForm.h

#ifndef _LIGHTCONTROLLERFORM_h
#define _LIGHTCONTROLLERFORM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "LCDForm.h"
#include "LightInterface.h"
#include "LCDStringList.h"
#include "LCDStringMsg.h"
#include "DisplayInterface.h"
#include "ChannelsBuffer.h"
#include "Timer.h"


#define LIGHT_WHEEL 0
#define ROOM_LIGHT 1
#define AWNING_LIGHT 2
#define WORKTOP_LIGHT 3
#define OFFSWITCH 4

class LightControllerForm : public LCDForm {
public:

	void init(Genie& genie);

	void update(Genie& genie);

	void onEvent(Genie& genie, genieFrame& evt);

	//TODO index needs to be adapted when the UI was build
	int getFormIndex() { return 4; }

private:

	double color;

	uint8_t roomBrightness;
	uint8_t awningBrightness;
	uint8_t worktopBrightness;

	void updateWidgetsValues(Genie& genie);

	Timer lastStateChange;
};

extern LightControllerForm lightControllerForm;

#endif
