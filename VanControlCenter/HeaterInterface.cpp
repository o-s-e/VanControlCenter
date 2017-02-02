#include "HeaterInterface.h"

void HeaterInterface::init() {
  pinMode(HT_GAS_PIN, OUTPUT);
  pinMode(HT_VENT_PIN, OUTPUT);
  pinMode(HT_STATE_PIN, OUTPUT);
  pinMode(HT_INPUT_PIN, INPUT);
  pinMode(HT_INPUT_PIN, OUTPUT);

  digitalWrite(HT_GAS_PIN, LOW);
  digitalWrite(HT_VENT_PIN, LOW);
  digitalWrite(HT_STATE_PIN, LOW);
  state = Off;

  lastStateUpdate.setDuration(HT_STATE_TTL).start();
}

void HeaterInterface::update() {
  if (lastStateUpdate.hasFinished()) {

    state = Unknown;
    lastStateUpdate.start();
  }
  if (ledTimer.hasFinished()) {

    switch (state) {

    case On:
      ledStatus = (ledStatus + 1) % 10;
      digitalWrite(HT_STATE_LED, ledStatus < 5 ? HIGH : LOW);
      digitalWrite(HT_GAS_PIN, HIGH);
      digitalWrite(HT_VENT_PIN, HIGH);
      Log.i(HT_TAG) << F("On state") << Endl;
      break;

    case Off:
      ledStatus = !ledStatus;
      digitalWrite(HT_STATE_LED, LOW);
      digitalWrite(HT_GAS_PIN, LOW);
      digitalWrite(HT_VENT_PIN, LOW);
      Log.i(HT_TAG) << F("Off state") << Endl;
      break;

    case VentOnly:
      ledStatus = (ledStatus + 1) % 10;
      digitalWrite(HT_STATE_LED, ledStatus < 20 ? HIGH : LOW);
      digitalWrite(HT_GAS_PIN, LOW);
      digitalWrite(HT_VENT_PIN, HIGH);
      Log.i(HT_TAG) << F("VentOnly state") << Endl;
      break;

    default:
      ledStatus = (ledStatus + 1) % 10;
      digitalWrite(HT_STATE_LED, ledStatus < 1 ? HIGH : LOW);
      digitalWrite(HT_GAS_PIN, LOW);
      digitalWrite(HT_VENT_PIN, LOW);
      Log.w(HT_TAG) << F("Unknown state") << Endl;
      break;
    }

    ledTimer.start();
  }
}

void HeaterInterface::onStateChanged(const char *newStateString) {
  HeaterState newState;

  if (strcmp(newStateString, ON_STATE) == 0) {
    newState = On;
  } else if (strcmp(newStateString, OFF_STATE) == 0) {
    newState = Off;
  } else if (strcmp(newStateString, VENT_STATE) == 0) {
    newState = VentOnly;
  } else if (strcmp(newStateString, ERROR_STATE) == 0) {
    newState = Error;
  } else {
    newState = Unknown;
  }

  if (newState != state) {
    ledStatus = 0;
    state = newState;
  }
  lastStateUpdate.start();
}

HeaterInterface heater
