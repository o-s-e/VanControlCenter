#ifndef TempSensor_h
#define TempSensor_h

#include <arduino.h>
#include <dht.h>
#include <logger.h>

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
