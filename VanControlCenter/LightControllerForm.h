// LightControllerForm.h

#ifndef LIGHTCONTROLLERFORM_H
#define LIGHTCONTROLLERFORM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
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

class LightControllerForm : public LcdForm {
public:

    void init(Genie& genie) override;

    void update(Genie& genie) override;

    void onEvent(Genie& genie, genieFrame& evt) override;

    //TODO index needs to be adapted when the UI was build
    int getFormIndex() override { return 1; }

private:

    double color_{};

    uint8_t roomBrightness_{};
    uint8_t awningBrightness_{};
    uint8_t worktopBrightness_{};

    void updateWidgetsValues(Genie& genie);

    Timer lastStateChange_;
};

extern LightControllerForm lightControllerForm;

#endif
