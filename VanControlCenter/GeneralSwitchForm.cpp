#include "GeneralSwitchForm.h"

void GeneralSwitchForm::init(Genie& genie)
{
	//TODO: Maybe we should fetch the switch states from the channelsbuffer
}

void GeneralSwitchForm::update(Genie& genie) 
{

	updateWidgetsValues(genie);
}

void GeneralSwitchForm::onEvent(Genie& genie, genieFrame& evt) {
	if (evt.reportObject.cmd == GENIE_REPORT_EVENT) {
		if (evt.reportObject.object == GENIE_OBJ_DIPSW) {
			switch (evt.reportObject.index) {
			case SWITCH_WATER:
				channelsBuffer.setValue<bool>(CanID::SW_WATER,
					genie.GetEventData(&evt));
				break;
			case SWITCH_1:
				channelsBuffer.setValue<bool>(CanID::SW_1,
					genie.GetEventData(&evt));
				break;

			}
		}

	}

}

void GeneralSwitchForm::updateWidgetsValues(Genie& genie)

{

}

GeneralSwitchForm generalSwitchForm;

