// 
// 
// 

#include "LightControllerForm.h"

void LightControllerForm::init(Genie& genie) {

	color = 255;
	brightness = 0;

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

				case LIGHT_BRIGHTNESS:
					brightness = genie.GetEventData(&evt);
					lightInterface.setBrightness(brightness);

			}
		}
	}
}

void LightControllerForm::updateWidgetsValues(Genie& genie) {

	updateWidget(genie, GENIE_OBJ_USERIMAGES, LIGHT_WHEEL, color);
	updateWidget(genie, GENIE_OBJ_USERIMAGES, LIGHT_BRIGHTNESS, brightness);

}

LightControllerForm lightControllerForm;

