#include "TemperatureControllerForm.h"

void TemperatureControllerForm::init(Genie& genie)
{
	//Initialize the temperature quiete high, so that the heater does not start by accident
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
					// Get the temp from the slider, but only store it in an instace variable, so we can set it later on the update function run

					channelsBuffer.setValue<double>(CanID::SET_TEMP, genie.GetEventData(&evt));
					break;
			}
		}
	}
}

void TemperatureControllerForm::updateWidgetsValues(Genie& genie) {
	//First we get the latest temp fronm the canbus buffer, then we call the heaterinterface on the value and update the view.
	double temp = channelsBuffer.getValueAs<double>(CanID::TEMP);
	//TODO move the set to the heatercontroller. Fetch here only the value from the channelsbuffer

	//if (temp >= setTemp) {
	//	heaterInterface.onStateChanged("VENT");
	//}
	//else if (temp < setTemp) {
	//	heaterInterface.onStateChanged("ON");
	//}
	//else {
		//Error
	//	Log.e(HT_TAG) << F("Could not set the temp") << Endl;
	//}

	updateWidget(genie, GENIE_OBJ_GAUGE, TEMP_GAUGE_DISP, temp);
}

TemperatureControllerForm temperatureControllerForm;