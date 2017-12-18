// LCDStringMsg.h

#ifndef LCDSTRINGMSG_H
#define LCDSTRINGMSG_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/**
* Class to use a string widget to show simple message
*/

#include <genieArduino.h>

#include "Utils.h"

class LcdStringMsg {
public:
    //Function to init.
    //Index is the ViSi editor string widget index
    void init(byte index, Genie* parent);

    //Clear the message
    void clear();

    //Get the string message
    const String& getMessage();

    //Repaint the message
    void repaint();

    //Set the message
    void setMessage(String str);
    void setMessage(const char* str);

private:
    //Widget ViSi index
    byte widgetIndex_;

    //String message
    String buffer_;

    //Genie instance pointer
    Genie* parent_;
};

#endif
