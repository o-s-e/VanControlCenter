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

    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_1, 0);
    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_2, 0);
    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_3, 0);
    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_4, 0);
    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_5, 0);
    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_6, 0);

    channelsBuffer.setValue<uint8_t>(CanID::HSV, 0);

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
    Log.i(RGB_TAG) << F("Light interface initialized") << Endl;

}

void LightInterfaceClass::update() {
    if (ledTimer.hasFinished()) {

        //debug hook

        if (channelsBuffer.getValueAs<double>(CanID::HSV) > 0) {
            Log.i(RGB_TAG) << F("hsv debug value set: ") << Endl;
            setColor(channelsBuffer.getValueAs<double>(CanID::HSV));
        }


        roomLight.r = channelsBuffer.getValueAs<uint8_t>(CanID::LIGHT_1);
        roomLight.g = channelsBuffer.getValueAs<uint8_t>(CanID::LIGHT_2);
        roomLight.b = channelsBuffer.getValueAs<uint8_t>(CanID::LIGHT_3);
        roomLight.w = channelsBuffer.getValueAs<uint8_t>(CanID::LIGHT_4);
        awningLight.w = channelsBuffer.getValueAs<uint8_t>(CanID::LIGHT_5);
        worktopLight.w = channelsBuffer.getValueAs<uint8_t>(CanID::LIGHT_6);

        // Set the color on each cycle
        analogWrite(RGB_RED, roomLight.r);
        analogWrite(RGB_BLUE, roomLight.g);
        analogWrite(RGB_GREEN, roomLight.b);
        analogWrite(WHITE_LED, roomLight.w);
        analogWrite(AWNING_LED, awningLight.w);
        analogWrite(WORKTOP_LED, worktopLight.w);





        ledTimer.start();
    }
}

void LightInterfaceClass::setColor(double h) {
    hsv.h = h;
    hsv.s = 0.5;
    hsv.v = 0.5;
    Log.i(RGB_TAG) << F("hsv: ") << hsv.h << F("|0.5|0.5") << Endl;

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
    Log.i(RGB_TAG) << F("tiny rgb: ") << r << F("|") << g << F("|") << b << Endl;

    // map the values from a 0-1 fraction to a byte
    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_1, mapf(r, 0, 1, 0, 255));
    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_2, mapf(g, 0, 1, 0, 255));
    channelsBuffer.setValue<uint8_t>(CanID::LIGHT_3, mapf(b, 0, 1, 0, 255));

    Log.i(RGB_TAG) << F("rgb: ") << channelsBuffer.getValueAsString(CanID::LIGHT_1) << F("|") << channelsBuffer.getValueAsString(CanID::LIGHT_2) << F("|") << channelsBuffer.getValueAsString(CanID::LIGHT_3) << Endl;
}

void LightInterfaceClass::setBrightness(uint8_t brightness, uint8_t lightIndex) {
    switch (lightIndex) {
        //TODO Check if roomlight needs to be replaced with the channelbuffer equivalent
    case 1:
        if (roomLight.w != brightness) {
            channelsBuffer.setValue<uint8_t>(CanID::LIGHT_4, brightness);;
        }
    case 2:
        if (awningLight.w != brightness) {
            channelsBuffer.setValue<uint8_t>(CanID::LIGHT_5, brightness);;;
        }
    case 3:
        if (worktopLight.w != brightness) {
            channelsBuffer.setValue<uint8_t>(CanID::LIGHT_6, brightness);;;
        }
    }
}

void LightInterfaceClass::allOff() {
    fadeAmount = 5;
    fadeTimer.setDuration(500).start();
    if (fadeTimer.hasFinished()) {
        roomLight.r = -fadeAmount;
        roomLight.g = -fadeAmount;
        roomLight.b = -fadeAmount;
        roomLight.w = -fadeAmount;
        awningLight.w = -fadeAmount;
        worktopLight.w = -fadeAmount;
        if (roomLight.w <= 0 &&
            roomLight.r <= 0 &&
            roomLight.g <= 0 &&
            roomLight.b <= 0 &&
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

double LightInterfaceClass::mapf(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

LightInterfaceClass lightInterface;