#include "TemperatureControllerForm.h"

void TemperatureControllerForm::init(Genie& genie)
{
	//Initialize the temperature quiete high, so that the heater does not start by accident
	setTemp = 25;
}

void TemperatureControllerForm::update(Genie& genie)
{
	updateWidgetsValues(genie);
}

void TemperatureControllerForm::onEvent(Genie& genie, genieFrame& evt) {
	if (evt.reportObject.cmd == GENIE_REPORT_EVENT) {
		if (evt.reportObject.object == GENIE_OBJ_GAUGE) {
			switch (evt.reportObject.index) {
				case TEMP_GAUGE_SET:
					// Get the temp from the slider
					setTemp = genie.GetEventData(&evt);
					break;
			}
		}
	}
}

void TemperatureControllerForm::updateWidgetsValues(Genie& genie) {
	float temp = channelsBuffer.getValueAs<float>(CanID::TEMP);
	//TODO add timer to avoid flicker
	if (temp >= setTemp) {
		heater.onStateChanged("OFF");
	}
	else if (temp < setTemp) {
		heater.onStateChanged("ON");
	}
	else {
		//Error
	}

	updateWidget(genie, GENIE_OBJ_GAUGE, TEMP_GAUGE_DISP, temp);
}

TemperatureControllerForm temperatureControllerForm;