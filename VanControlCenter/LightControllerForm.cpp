//
//
//

#include "LightControllerForm.h"

void LightControllerForm::init(Genie& genie) {
    //TODO compare with channelbuffer
    color_ = 0; // 0-360
    roomBrightness_ = 0; // 0-255
    awningBrightness_ = 0;
    worktopBrightness_ = 0;
}

void LightControllerForm::update(Genie& genie) {
    updateWidgetsValues(genie);
}

void LightControllerForm::onEvent(Genie& genie, genieFrame& evt) {
    if (evt.reportObject.cmd == GENIE_REPORT_EVENT) {
        if (evt.reportObject.object == GENIE_OBJ_USERIMAGES) {
            switch (evt.reportObject.index) {
            case LIGHT_WHEEL:
                //Get the angle value to set the color
                color_ = genie.GetEventData(&evt);
                lightInterface.setColor(color_);
                break;
            case ROOM_LIGHT:
                roomBrightness_ = genie.GetEventData(&evt);
                lightInterface.setBrightness(roomBrightness_, ROOM_LIGHT);
                break;
            case AWNING_LIGHT:
                awningBrightness_ = genie.GetEventData(&evt);
                lightInterface.setBrightness(awningBrightness_, AWNING_LIGHT);
                break;
            case WORKTOP_LIGHT:
                worktopBrightness_ = genie.GetEventData(&evt);
                lightInterface.setBrightness(worktopBrightness_, WORKTOP_LIGHT);
                break;
            case OFFSWITCH:
                lightInterface.allOff();
                break;
            default:
                Log.w(LCD_TAG) << F("Display returned a not used object id on the LightControllerForm: ") << evt.
                    reportObject.index << Endl;
            }
        }
    }
}

void LightControllerForm::updateWidgetsValues(Genie& genie) {
    updateWidget(genie, GENIE_OBJ_USERIMAGES, LIGHT_WHEEL, color_);
    updateWidget(genie, GENIE_OBJ_USERIMAGES, ROOM_LIGHT, roomBrightness_);
    updateWidget(genie, GENIE_OBJ_USERIMAGES, AWNING_LIGHT, awningBrightness_);
    updateWidget(genie, GENIE_OBJ_USERIMAGES, WORKTOP_LIGHT, worktopBrightness_);
}

LightControllerForm lightControllerForm;
