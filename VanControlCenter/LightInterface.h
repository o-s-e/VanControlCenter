// LightInterface.h

#ifndef _LIGHTINTERFACE_h
#define _LIGHTINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "HWConfig.h"
#include <Timer.h>
#include "Logger.h"
#include "ChannelsBuffer.h"
#include "CanInterface.h"

/**
* Class to interface the RGB cabin lightning
*/

//Logger Tag
#define RGB_TAG F("RGB")

#define RGB_STATE_LED_DUR 500

typedef struct LIGHT {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t w;
}LIGHT;

typedef struct HSV {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1 we use a static 0.5
	double v;       // a fraction between 0 and 1 we use a static 0.5
} HSV;

class LightInterfaceClass {
public:
	//Function to call in the setup
	void init();

	//Function to call in the loop
	void update();

	// Sets the Hue(value between 0 and 360)
	void setColor(double h);

	void setBrightness(uint8_t brightness, uint8_t lightIndex);

	void allOff();

private:

	LIGHT roomLight;
	LIGHT awningLight;
	LIGHT worktopLight;

	HSV hsv;

	uint8_t fadeAmount;

	// LightCtrl refresh LED timer
	Timer ledTimer;
	Timer fadeTimer;
};

extern LightInterfaceClass lightInterface;

#endif
