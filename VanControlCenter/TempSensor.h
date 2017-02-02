#ifndef TempSensor_h
#define TempSensor_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


#include "logger.h"

#include <dht.h>

// Logger tag
#define T_SEN_TAG F("TempCSensor")

class TempSensor {
public:
  TempSensor(uint8_t i_pin);
  double update(unsigned long currentMillis);

private:
  uint8_t sensorPin;
  unsigned long interval;
  unsigned long previousMillis;
  double temp;
};

#endif
