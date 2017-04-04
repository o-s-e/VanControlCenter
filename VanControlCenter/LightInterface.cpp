// 
// 
// 

#include "LightInterface.h"


void LightInterfaceClass::init() {

	roomLight.r = 0;
	roomLight.g = 0;
	roomLight.b = 0;
	roomLight.w = 0;

	awningLight.w = 0;

	worktopLight.w = 0;

	hsv.h = 0;
	hsv.s = 0;
	hsv.v = 0;


	pinMode(RGB_RED, OUTPUT);
	pinMode(RGB_BLUE, OUTPUT);
	pinMode(RGB_GREEN, OUTPUT);
	pinMode(WHITE_LED, OUTPUT);
	pinMode(AWNING_LED, OUTPUT);
	pinMode(WORKTOP_LED, OUTPUT);

	analogWrite(RGB_RED, 0);
	analogWrite(RGB_BLUE, 0);
	analogWrite(RGB_GREEN, 0);
	analogWrite(WHITE_LED, 0);
	analogWrite(AWNING_LED, 0);
	analogWrite(WORKTOP_LED, 0);

	ledTimer.setDuration(RGB_STATE_LED_DUR).start();

}

void LightInterfaceClass::update() {

	if (ledTimer.hasFinished()) {

		// Set the color on each cycle
		analogWrite(RGB_RED, roomLight.r);
		analogWrite(RGB_BLUE, roomLight.g);
		analogWrite(RGB_GREEN, roomLight.b);
		analogWrite(WHITE_LED, roomLight.w);
		analogWrite(AWNING_LED, awningLight.w);
		analogWrite(WORKTOP_LED, worktopLight.w);
	}

	ledTimer.start();

}

void LightInterfaceClass::setColor(double h) {
	hsv.h = h;
	hsv.s = 0.5;
	hsv.v = 0.5;

	double      hh, p, q, t, ff, r, g, b;
	long        i;

	if (hsv.s <= 0.0) {
		r = hsv.v;
		g = hsv.v;
		b = hsv.v;
	}
	else
	{

		hh = hsv.h;
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = hsv.v * (1.0 - hsv.s);
		q = hsv.v * (1.0 - (hsv.s * ff));
		t = hsv.v * (1.0 - (hsv.s * (1.0 - ff)));

		switch (i) {
			case 0:
				r = hsv.v;
				g = t;
				b = p;
				break;

			case 1:
				r = q;
				g = hsv.v;
				b = p;
				break;
			case 2:
				r = p;
				g = hsv.v;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = hsv.v;
				break;
			case 4:
				r = t;
				g = p;
				b = hsv.v;
				break;
			case 5:
			default:
				r = hsv.v;
				g = p;
				b = q;
				break;
		}
	}
	// map the values from a 0-1 fraction to a byte
	roomLight.r = map(r, 0, 1, 0, 255);
	roomLight.g = map(g, 0, 1, 0, 255);
	roomLight.b = map(b, 0, 1, 0, 255);
}

void LightInterfaceClass::setBrightness(int brightness, int lightIndex) {
	switch (lightIndex) {
		case 1:
			if (roomLight.w != brightness) {
				roomLight.w = brightness;
			}
		case 2:
			if (awningLight.w != brightness) {
				awningLight.w = brightness;
			}
		case 3:
			if (worktopLight.w != brightness) {
				worktopLight.w = brightness;
			}

	}
}

void LightInterfaceClass::allOff() {
	fadeAmount = 5;
	fadeTimer.setDuration(500).start();
	if (ledTimer.hasFinished()) {


		roomLight.r = -fadeAmount;
		roomLight.g = -fadeAmount;
		roomLight.b = -fadeAmount;
		roomLight.w = -fadeAmount;
		awningLight.w = -fadeAmount;
		worktopLight.w = -fadeAmount;
		if (roomLight.r <= 0 &&
			roomLight.g <= 0 &&
			roomLight.b <= 0 &&
			roomLight.w <= 0 &&
			awningLight.w <= 0 &&
			worktopLight.w <= 0) {
			Log.i(RGB_TAG) << F("all light are off");
		}
		else
		{
			fadeTimer.start();

		}
	}
}


LightInterfaceClass lightInterface;
