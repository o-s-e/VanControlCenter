//
//
//

#include "LightControllerForm.h"

void LightControllerForm::init(Genie& genie) {
	color = 0; // 0-360
	roomBrightness = 0; // 0-255
	awningBrightness = 0;
	worktopBrightness = 0;
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
					color = genie.GetEventData(&evt);
					lightInterface.setColor(color);

				case ROOM_LIGHT:
					roomBrightness = genie.GetEventData(&evt);
					lightInterface.setBrightness(roomBrightness, ROOM_LIGHT);

				case AWNING_LIGHT:
					awningBrightness = genie.GetEventData(&evt);
					lightInterface.setBrightness(awningBrightness, AWNING_LIGHT);

				case WORKTOP_LIGHT:
					worktopBrightness = genie.GetEventData(&evt);
					lightInterface.setBrightness(worktopBrightness, WORKTOP_LIGHT);

				case OFFSWITCH:
					lightInterface.allOff();
			}
		}
	}
}

void LightControllerForm::updateWidgetsValues(Genie& genie) {
	updateWidget(genie, GENIE_OBJ_USERIMAGES, LIGHT_WHEEL, color);
	updateWidget(genie, GENIE_OBJ_USERIMAGES, ROOM_LIGHT, roomBrightness);
	updateWidget(genie, GENIE_OBJ_USERIMAGES, AWNING_LIGHT, awningBrightness);
	updateWidget(genie, GENIE_OBJ_USERIMAGES, WORKTOP_LIGHT, worktopBrightness);
}

LightControllerForm lightControllerForm;