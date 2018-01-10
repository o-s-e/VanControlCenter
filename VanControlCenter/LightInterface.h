// LightInterface.h

#ifndef LIGHTINTERFACE_H
#define LIGHTINTERFACE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "HWConfig.h"
#include <Timer.h>
#include "logger.h"
#include "ChannelsBuffer.h"
#include "CanInterface.h"

/**
* Class to interface the RGB cabin lightning
*/

//Logger Tag
#define RGB_TAG F("Lightning")

#define RGB_STATE_LED_DUR 1000

using light = struct Light {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
};

using hsv = struct Hsv {
    uint_fast8_t h; // angle in degrees
    double s; // a fraction between 0 and 1 we use a static 0.5
    double v; // a fraction between 0 and 1 we use a static 0.5
};

class LightInterfaceClass {  // NOLINT
public:
    //Function to call in the setup
    void init();

    //Function to call in the loop
    void update();

    // Sets the Hue(value between 0 and 360)
    void setColor(uint_fast8_t h);

    void setBrightness(uint8_t brightness, uint8_t lightIndex);

    void allOff();

private:

    double mapf(double x, double in_min, double in_max, double out_min, double out_max);

    light roomLight_;
    light awningLight_;
    light worktopLight_;

    hsv hsv_;

    uint8_t fadeAmount_ = 0;

    // LightCtrl refresh LED timer
    Timer ledTimer_;
    Timer fadeTimer_;
};

extern LightInterfaceClass lightInterface;

#endif
