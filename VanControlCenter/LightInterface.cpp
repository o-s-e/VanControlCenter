// 
// 
// 

#include "LightInterface.h"


void LightInterfaceClass::init() {

	light.r = 0;
	light.g = 0;
	light.b = 0;

	hsv.h = 0;
	hsv.s = 0;
	hsv.v = 0;



	pinMode(RGB_RED, OUTPUT);
	pinMode(RGB_BLUE, OUTPUT);
	pinMode(RGB_GREEN, OUTPUT);
	pinMode(WHITE_LED, OUTPUT);

	analogWrite(RGB_RED, 0);
	analogWrite(RGB_BLUE, 0);
	analogWrite(RGB_GREEN, 0);
	analogWrite(WHITE_LED, 0);

	ledTimer.setDuration(RGB_STATE_LED_DUR).start();


}

void LightInterfaceClass::update() {

	if (ledTimer.hasFinished()) {

		// Set the color on each cycle
		analogWrite(RGB_RED, light.r);
		analogWrite(RGB_BLUE, light.g);
		analogWrite(RGB_GREEN, light.b);
		analogWrite(WHITE_LED, light.w);
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
	light.r = map(r, 0, 1, 0, 255);
	light.g = map(g, 0, 1, 0, 255);
	light.b = map(b, 0, 1, 0, 255);
}

void LightInterfaceClass::setBrightness(int h) {
	if (light.w != h) {
		light.w = h;
	}

}


LightInterfaceClass lightInterface;
