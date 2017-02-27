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


/**
* Class to interface the RGB cabin lightning
*/

//Logger Tag
#define RGB_TAG F("RGB")

#define RGB_STATE_LED_DUR 500

typedef struct LIGHT {
	double r;       
	double g;       
	double b;
	int w;
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

	void setBrightness(int h);

	


private:


	LIGHT light;
	HSV hsv;

	// LightCtrl state LED timer
	Timer ledTimer;



};

extern LightInterfaceClass LightInterface;

#endif
