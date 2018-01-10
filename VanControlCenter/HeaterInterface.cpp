#include "HeaterInterface.h"

void HeaterInterface::init() {
    pinMode(HT_GAS_PIN, OUTPUT);
    pinMode(HT_VENT_PIN, OUTPUT);
    pinMode(HT_STATE_LED, OUTPUT);

    digitalWrite(HT_GAS_PIN, LOW);
    digitalWrite(HT_VENT_PIN, LOW);
    digitalWrite(HT_STATE_LED, LOW);
    state_ = OFF;
    ledTimer_.setDuration(HT_STATE_LED_DUR).start();
    lastStateUpdate_.setDuration(HT_STATE_TTL).start();

    heaterFaultCode_ = 1;

    Log.i(HT_TAG) << F("Heater Interface initialized") << Endl;
}

void HeaterInterface::update() {
    if (lastStateUpdate_.hasFinished()) {
        auto temp = channelsBuffer.getValueAs<double>(CanId::TEMP);
        auto setTemp = channelsBuffer.getValueAs<double>(CanId::SET_TEMP);

        if (temp >= setTemp) {
            if (state_ == ON) {
                cooldown();
            }
        }
        else if (temp < setTemp) {
            state_ = ON;
            Log.i(HT_TAG) << F("State: On: ") << state_ << Endl;

        }
        else {
            //Error
            Log.e(HT_TAG) << F("Could not set the temp") << Endl;
        }

        lastStateUpdate_.start();
    }
    if (ledTimer_.hasFinished()) {
        switch (state_) {
        case ON:
            ledStatus_ = (ledStatus_ + 1) % 10;
            digitalWrite(HT_STATE_LED, ledStatus_ < 5 ? HIGH : LOW);
            digitalWrite(HT_GAS_PIN, HIGH);
            digitalWrite(HT_VENT_PIN, HIGH);
            break;

        case OFF:
            ledStatus_ = !ledStatus_;
            digitalWrite(HT_STATE_LED, LOW);
            digitalWrite(HT_GAS_PIN, LOW);
            digitalWrite(HT_VENT_PIN, LOW);
            break;

        case VENT_ONLY:
            ledStatus_ = (ledStatus_ + 1) % 10;
            digitalWrite(HT_STATE_LED, ledStatus_ < 20 ? HIGH : LOW);
            digitalWrite(HT_GAS_PIN, LOW);
            digitalWrite(HT_VENT_PIN, HIGH);
            // not the good way, but it works
            heaterInterface.cooldown();
            break;

        default:
            ledStatus_ = (ledStatus_ + 1) % 10;
            digitalWrite(HT_STATE_LED, ledStatus_ < 1 ? HIGH : LOW);
            digitalWrite(HT_GAS_PIN, LOW);
            digitalWrite(HT_VENT_PIN, LOW);
            break;
        }
        channelsBuffer.setValue<uint8_t>(CanId::HEATER_STATUS, state_);

        ledTimer_.start();

    }

}

void HeaterInterface::onStateChanged(const char* newStateString) {
    HeaterState newState;

    if (strcmp(newStateString, ON_STATE) == 0) {
        newState = ON;
    }
    else if (strcmp(newStateString, OFF_STATE) == 0) {
        newState = OFF;
    }
    else if (strcmp(newStateString, VENT_STATE) == 0) {
        newState = VENT_ONLY;
    }
    else if (strcmp(newStateString, ERROR_STATE) == 0) {
        newState = ERROR;
    }
    else {
        newState = UNKNOWN;
    }

    if (newState != state_) {
        ledStatus_ = 0;
        state_ = newState;
    }
    lastStateUpdate_.start();
}

void HeaterInterface::cooldown() {
    Log.i(HT_TAG) << F("CooldownTimer elapsed time: ") << cooldownTimer_.elapsedTime() << Endl;
    Log.i(HT_TAG) << F("CooldownTimer has finished?: ") << cooldownTimer_.hasFinished() << Endl;
    if (state_ == ON) {
        Log.i(HT_TAG) << F("Colldownsquence initiated") << Endl;
        cooldownTimer_.setDuration(HT_COOLDOWN_DUR).start();
        state_ = VENT_ONLY;
    }
    else if (state_ == VENT_ONLY && cooldownTimer_.isRunning()) {

        Log.i(HT_TAG) << F("State: ") << state_ << Endl;
        Log.i(HT_TAG) << F("CoolDown") << Endl;
    }
    else if (state_ == VENT_ONLY && cooldownTimer_.hasFinished()) {
        Log.i(HT_TAG) << F("State: ") << state_ << Endl;
        Log.i(HT_TAG) << F("Stopping Heater") << Endl;
        state_ = OFF;
    }
    else {
        Log.i(HT_TAG) << F("State: ") << state_ << Endl;
        Log.e(HT_TAG) << F("Undefined State") << Endl;
    }

}

//TODO: One must adapt the timer, so that the interupts are only counted from one cycle
void HeaterInterface::heaterFaultCodeCallback() {
    if (heaterFaultCode_ > 0 && heaterFaultCode_ < 12)
        heaterFaultCode_ += 1;
    else if (heaterFaultCode_ == 12)
        heaterFaultCode_ = 12;
}

HeaterInterface heaterInterface;
