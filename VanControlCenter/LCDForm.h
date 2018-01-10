// LCDForm.h

#ifndef LCDFORM_H
#define LCDFORM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
* Abstract class representing a generic lcd form
*/

#include <genieArduino.h>

class LcdForm {
protected:

    //Function wrapper to write a value on a widget
    void updateWidget(Genie& genie, uint16_t wType, uint16_t wIndex, uint16_t wValue) {
        genie.WriteObject(wType, wIndex, wValue);
    }

    //Function wrapper to write a string
    void updateString(Genie& genie, uint16_t wIndex, const String& wValue) {
        genie.WriteStr(wIndex, const_cast<char*>(wValue.c_str()));
    }

public:
    virtual ~LcdForm();
    //Function to call to init the form
    virtual void init(Genie& genie);

    //Function called when the form is updated
    virtual void update(Genie& genie);

    //Function called when an event is received from the lcd
    virtual void onEvent(Genie& genie, genieFrame& evt) {}

    //Function called when the form become active
    virtual void onEnter(Genie& genie) {}

    //Function called when the form become inactive
    virtual void onExit(Genie& genie) {}

    //Get form index in ViSi
    virtual int getFormIndex();
};

inline LcdForm::~LcdForm() = default;
inline void LcdForm::init(Genie& genie) {}
inline void LcdForm::update(Genie& genie) {}
inline int LcdForm::getFormIndex() { return 0; }
#endif
