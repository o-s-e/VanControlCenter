// ConsoleForm.h

#ifndef _CONSOLEFORM_h
#define _CONSOLEFORM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
* Class representing the first form shown when the lcd is boot
*/

#include "DisplayInterface.h"
#include "LCDForm.h"

// Console text max size
#define TEXT_BUFFER_SIZE 256

// Console text max cols
#define TEXT_COLS 58

// Console text max rows
#define TEXT_ROWS 22

// Console text string widget index
#define CONSOLE_STR 0

class ConsoleFormClass : public LcdForm {
public:
    // LCDForm abstract method implementation
    void init(Genie& genie) override;

    // LCDForm abstract method implementation
    void update(Genie& genie) override;

    // Function to print on the console text widget
    void print(const String& str);

    // Function to print on the console text widget
    void println(const String& str);

    // Function to clear the console text widget
    void clear();

    // LCDForm abstract method implementation
    int getFormIndex() override { return 0; }

private:
    // Genie instance pointer
    Genie* genie_{};

    // Text buffer
    String text_;

    // Function to add text to the buffer
    void addText(const String& str);
};

// Console form instance
extern ConsoleFormClass consoleForm;

#endif
