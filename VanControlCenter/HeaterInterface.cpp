#include "HeaterInterface.h"

void HeaterInterface::init() {
    pinMode(HT_GAS_PIN, OUTPUT);
    pinMode(HT_VENT_PIN, OUTPUT);
    pinMode(HT_STATE_LED, OUTPUT);

    digitalWrite(HT_GAS_PIN, LOW);
    digitalWrite(HT_VENT_PIN, LOW);
    digitalWrite(HT_STATE_LED, LOW);
    state = Off;
    ledTimer.setDuration(HT_STATE_LED_DUR).start();
    lastStateUpdate.setDuration(HT_STATE_TTL).start();

    heaterFaultCode = 1;

    Log.i(HT_TAG) << F("Heater Interface initialized") << Endl;
}

void HeaterInterface::update() {
    if (lastStateUpdate.hasFinished()) {
        double temp = channelsBuffer.getValueAs<double>(CanID::TEMP);
        double setTemp = channelsBuffer.getValueAs<double>(CanID::SET_TEMP);

        if (temp >= setTemp) {
            if (state == On) {
                cooldown();
            }
        }
        else if (temp < setTemp) {
            state = On;
            Log.i(HT_TAG) << F("State: On: ") << state << Endl;

        }
        else {
            //Error
            Log.e(HT_TAG) << F("Could not set the temp") << Endl;
        }

        lastStateUpdate.start();
    }
    if (ledTimer.hasFinished()) {
        switch (state) {
        case On:
            ledStatus = (ledStatus + 1) % 10;
            digitalWrite(HT_STATE_LED, ledStatus < 5 ? HIGH : LOW);
            digitalWrite(HT_GAS_PIN, HIGH);
            digitalWrite(HT_VENT_PIN, HIGH);
            break;

        case Off:
            ledStatus = !ledStatus;
            digitalWrite(HT_STATE_LED, LOW);
            digitalWrite(HT_GAS_PIN, LOW);
            digitalWrite(HT_VENT_PIN, LOW);
            break;

        case VentOnly:
            ledStatus = (ledStatus + 1) % 10;
            digitalWrite(HT_STATE_LED, ledStatus < 20 ? HIGH : LOW);
            digitalWrite(HT_GAS_PIN, LOW);
            digitalWrite(HT_VENT_PIN, HIGH);
            // not the good way, but it works
            heaterInterface.cooldown();
            break;

        default:
            ledStatus = (ledStatus + 1) % 10;
            digitalWrite(HT_STATE_LED, ledStatus < 1 ? HIGH : LOW);
            digitalWrite(HT_GAS_PIN, LOW);
            digitalWrite(HT_VENT_PIN, LOW);
            break;
        }

        ledTimer.start();

    }

}

void HeaterInterface::onStateChanged(const char *newStateString) {
    HeaterState newState;

    if (strcmp(newStateString, ON_STATE) == 0) {
        newState = On;
    }
    else if (strcmp(newStateString, OFF_STATE) == 0) {
        newState = Off;
    }
    else if (strcmp(newStateString, VENT_STATE) == 0) {
        newState = VentOnly;
    }
    else if (strcmp(newStateString, ERROR_STATE) == 0) {
        newState = Error;
    }
    else {
        newState = Unknown;
    }

    if (newState != state) {
        ledStatus = 0;
        state = newState;
    }
    lastStateUpdate.start();
}

void HeaterInterface::cooldown() {
    Log.i(HT_TAG) << F("CooldownTimer elapsed time: ") << cooldownTimer.elapsedTime() << Endl;
    Log.i(HT_TAG) << F("CooldownTimer has finished?: ") << cooldownTimer.hasFinished() << Endl;
    if (state == On) {
        Log.i(HT_TAG) << F("Colldownsquence initiated") << Endl;
        cooldownTimer.setDuration(HT_COOLDOWN_DUR).start();
        state = VentOnly;
    }
    else if (state == VentOnly && cooldownTimer.isRunning()) {

        Log.i(HT_TAG) << F("State: ") << state << Endl;
        Log.i(HT_TAG) << F("CoolDown") << Endl;
    }
    else if (state == VentOnly && cooldownTimer.hasFinished()) {
        Log.i(HT_TAG) << F("State: ") << state << Endl;
        Log.i(HT_TAG) << F("Stopping Heater") << Endl;
        state = Off;
    }
    else {
        Log.i(HT_TAG) << F("State: ") << state << Endl;
        Log.e(HT_TAG) << F("Undefined State") << Endl;
    }

}

//TODO: One must adapt the timer, so that the interupts are only counted from one cycle
void HeaterInterface::heaterFaultCodeCallback() {
    if (heaterFaultCode > 0 && heaterFaultCode < 12)
        heaterFaultCode += 1;
    else if (heaterFaultCode == 12)
        heaterFaultCode = 12;
}

HeaterInterface heaterInterface;