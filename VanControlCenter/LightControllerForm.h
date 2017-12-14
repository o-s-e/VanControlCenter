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

#define LIGHT_WHEEL 3
#define ROOM_LIGHT 0
#define AWNING_LIGHT 1
#define WORKTOP_LIGHT 2
#define OFFSWITCH 4

class LightControllerForm : public LCDForm {
public:

    void init(Genie& genie) override;

    void update(Genie& genie) override;

    void onEvent(Genie& genie, genieFrame& evt) override;

    //TODO index needs to be adapted when the UI was build
    int getFormIndex() override { return 1; }

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
