#include "TempSensor.h"

dht DHT;

TempSensor::TempSensor(uint8_t i_pin) {
  sensorPin = i_pin;
  previousMillis = 0;
  interval = 5000;
  temp = 0.00;

  //  pinMode(sensorPin, INPUT);
}

double TempSensor::update(unsigned long currentMillis) {
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Log.w(T_SEN_TAG) << F("DHT update") << Endl;

    int chk = DHT.read11(sensorPin);
    switch (chk) {
    case DHTLIB_OK:
      Log.i(T_SEN_TAG) << F("OK") << Endl;
      temp = DHT.temperature;
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Log.e(T_SEN_TAG) << F("Checksum error") << Endl;
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Log.e(T_SEN_TAG) << F("Timeout error") << Endl;
      break;
    default:
      Log.e(T_SEN_TAG) << F("Unknown error") << Endl;
      break;
    }
    Log.i(T_SEN_TAG) << F("Current sensor temp: ") << temp << Endl;

    return temp;
  }
}
