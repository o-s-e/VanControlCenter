//
//
//

#include "LightInterface.h"

void LightInterfaceClass::init() {
    roomLight_.r = 0;
    roomLight_.g = 0;
    roomLight_.b = 0;
    roomLight_.w = 0;

    awningLight_.w = 0;

    worktopLight_.w = 0;

    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_1, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_2, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_3, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_4, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_5, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_6, 0);

    channelsBuffer.setValue<uint_fast16_t>(CanId::HSV, 0);

    hsv_.h = 0;
    hsv_.s = 0;
    hsv_.v = 0;

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

    ledTimer_.setDuration(RGB_STATE_LED_DUR).start();
    Log.i(RGB_TAG) << F("Light interface initialized") << Endl;

}

void LightInterfaceClass::update() {
    if (ledTimer_.hasFinished()) {

        //debug hook

        if (channelsBuffer.getValueAs<uint_fast16_t>(CanId::HSV) > 0 && channelsBuffer.getValueAs<uint_fast16_t
        >(CanId::HSV) <= 360) {
            Log.i(RGB_TAG) << F("hsv debug value set: ") << channelsBuffer.getValueAs<uint_fast16_t>(CanId::HSV) <<
                Endl;
            setColor(channelsBuffer.getValueAs<uint_fast16_t>(CanId::HSV));
        }
        else if (channelsBuffer.getValueAs<uint_fast16_t>(CanId::HSV) == 361) {
            allOff();
        }
        else {
            setColor(0);

        }


        roomLight_.r = channelsBuffer.getValueAs<uint8_t>(CanId::LIGHT_1);
        roomLight_.g = channelsBuffer.getValueAs<uint8_t>(CanId::LIGHT_2);
        roomLight_.b = channelsBuffer.getValueAs<uint8_t>(CanId::LIGHT_3);
        roomLight_.w = channelsBuffer.getValueAs<uint8_t>(CanId::LIGHT_4);
        awningLight_.w = channelsBuffer.getValueAs<uint8_t>(CanId::LIGHT_5);
        worktopLight_.w = channelsBuffer.getValueAs<uint8_t>(CanId::LIGHT_6);

        // Set the color on each cycle
        analogWrite(RGB_RED, roomLight_.r);
        analogWrite(RGB_BLUE, roomLight_.g);
        analogWrite(RGB_GREEN, roomLight_.b);
        analogWrite(WHITE_LED, roomLight_.w);
        analogWrite(AWNING_LED, awningLight_.w);
        analogWrite(WORKTOP_LED, worktopLight_.w);

        ledTimer_.start();
    }
}

void LightInterfaceClass::setColor(uint_fast16_t h) {
    hsv_.h = h;
    hsv_.s = 0.5;
    hsv_.v = 0.5;

    double hh, p, q, t, ff, r, g, b;
    long i;

    if (hsv_.s <= 0.0) {
        r = hsv_.v;
        g = hsv_.v;
        b = hsv_.v;
    }
    else {
        hh = static_cast<double>(hsv_.h);
        if (hh >= 360.0) hh = 0.0;
        hh /= 60.0;
        i = static_cast<long>(hh);
        ff = hh - i;
        p = hsv_.v * (1.0 - hsv_.s);
        q = hsv_.v * (1.0 - (hsv_.s * ff));
        t = hsv_.v * (1.0 - (hsv_.s * (1.0 - ff)));

        switch (i) {
        case 0:
            r = hsv_.v;
            g = t;
            b = p;
            break;

        case 1:
            r = q;
            g = hsv_.v;
            b = p;
            break;
        case 2:
            r = p;
            g = hsv_.v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = hsv_.v;
            break;
        case 4:
            r = t;
            g = p;
            b = hsv_.v;
            break;
        case 5:
        default:
            r = hsv_.v;
            g = p;
            b = q;
            break;
        }
    }
    // map the values from a 0-1 fraction to a byte
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_1, mapf(r, 0, 1, 0, 255));
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_2, mapf(g, 0, 1, 0, 255));
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_3, mapf(b, 0, 1, 0, 255));

}

void LightInterfaceClass::setBrightness(uint8_t brightness, uint8_t lightIndex) {
    switch (lightIndex) {
        //TODO Check if roomlight needs to be replaced with the channelbuffer equivalent
    case 1:
        if (roomLight_.w != brightness) {
            channelsBuffer.setValue<uint8_t>(CanId::LIGHT_4, brightness);
        }
        break;
    case 2:
        if (awningLight_.w != brightness) {
            channelsBuffer.setValue<uint8_t>(CanId::LIGHT_5, brightness);
        }
        break;
    case 3:
        if (worktopLight_.w != brightness) {
            channelsBuffer.setValue<uint8_t>(CanId::LIGHT_6, brightness);
        }
        break;
    default:
        Log.w(RGB_TAG) << F("not used lightIndex on the LightInterface: ") << lightIndex << Endl;
        break;
    }
}

void LightInterfaceClass::allOff() {
    Log.i(RGB_TAG) << F("all light are shutting down") << Endl;

    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_1, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_2, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_3, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_4, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_5, 0);
    channelsBuffer.setValue<uint8_t>(CanId::LIGHT_6, 0);

    Log.i(RGB_TAG) << F("all light are off") << Endl;
}


uint8_t LightInterfaceClass::mapf(double x, double in_min, double in_max, double out_min, double out_max) {
    return static_cast<uint8_t>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

LightInterfaceClass lightInterface;
