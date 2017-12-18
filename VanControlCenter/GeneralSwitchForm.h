// GeneralSwitchForm.h

#ifndef GENERALSWITCHFORM_H
#define GENERALSWITCHFORM_H

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

class GeneralSwitchForm : public LcdForm {
public:

    void init(Genie& genie) override;

    void update(Genie& genie) override;

    void onEvent(Genie& genie, genieFrame& evt) override;

    //TODO index needs to be adapted when the UI was build
    int getFormIndex() override { return 4; }

private:

    //Update all the widgets values

    void updateWidgetsValues(Genie& genie);

    Timer lastStateChange_;
};
extern GeneralSwitchForm generalSwitchForm;

#endif

