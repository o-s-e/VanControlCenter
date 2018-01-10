#include "DisplayInterface.h"

void onEvent() {
    genieFrame evt{};
    // Get the next pending event
    displayInterface.genie_.DequeueEvent(&evt);

    // If current form is valid
    if (displayInterface.currentForm_ != NULL) {
        // Redirect the screen event on the current form invoking onEvent method
        displayInterface.currentForm_->onEvent(displayInterface.genie_, evt);
    }
}

void DisplayInterfaceClass::init() {

    // Init the serial comunication
    INIT_SERIAL(LCD_SERIAL, LCD_SERIAL_BAUD);
    while (!genie_.Begin(LCD_SERIAL)) {
        Log.w(LCD_TAG) << F("Display not initialized") << Endl;
    }
    if (genie_.online()) {
        Log.w(LCD_TAG) << F("Display online") << Endl;
    }
    genie_.debug(LOG_SERIAL, 6);

    // Reset the LCD
    pinMode(LCD_RESET_PIN, OUTPUT);
    digitalWrite(LCD_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LCD_RESET_PIN, HIGH);
    delay(4500);


    // Attach event handler method
    genie_.AttachEventHandler(&onEvent);

    // set Brightness
    genie_.WriteContrast(10);

    // Init forms
    consoleForm.init(genie_);
    //debugForm.init(genie);
    temperatureControllerForm.init(genie_);
    lightControllerForm.init(genie_);
    generalSwitchForm.init(genie_);
    currentForm_ = &consoleForm;

    // Refresh rate for the update method
    refreshTimer_.setDuration(1000 / REFRESH_RATE).start();

    enabled_ = true;
}

void DisplayInterfaceClass::update() {

    // Update events
    genie_.DoEvents();

    // If it's refresh time then update current form

    switch (genie_.form()) {
    case CONSOLEFORM:
        setCurrentForm(&consoleForm);
        break;
    case LIGHTCTRLFORM:
        setCurrentForm(&lightControllerForm);
        break;
    case TEMPCTRLFORM:
        setCurrentForm(&temperatureControllerForm);
        break;
    case GENERALFORM:
        setCurrentForm(&generalSwitchForm);
        break;
    case DEBUGFORM:
        setCurrentForm(&debugForm);
        break;
    default:
        setCurrentForm(&consoleForm);
        Log.w(LCD_TAG) << F("default form called: ") << genie_.form() << Endl;
        break;
    }

    if (enabled_ && refreshTimer_.hasFinished()) {
        if (currentForm_ != NULL) {
            currentForm_->update(genie_);
        }

        // If the refresh time is high disable the future updates. Probably the
        //screen froze or shut down

        if (refreshTimer_.elapsedTime() > MAX_UPDATE_TIME) {
            enabled_ = false;
            Log.w(LCD_TAG) << F("Disabled LCD updates") << Endl;
        }


        refreshTimer_.start();
    }
}

void DisplayInterfaceClass::setCurrentForm(LcdForm* currentForm) {

    const char* formTitles[] = {
        "ConsoleForm", "LightcontrollerForm", "TemperatureControllerForm", "GeneralForm", "DebugForm"
    };

    // If current form is not null invoke onExit method
    if (currentForm != NULL) {
        this->currentForm_->onExit(genie_);
    }

    this->currentForm_ = currentForm;

    if (currentForm != NULL) {
        // Activate next form
        genie_.form(currentForm->getFormIndex());
        // Invoke new current form onEnter method
        currentForm->onEnter(genie_);
        // Update new current form
        currentForm->update(genie_);
        // Restart refresh timer
        refreshTimer_.start();
    }
    Log.i(LCD_TAG) << F("Current display form set to: ") << formTitles[genie_.form()] << Endl;
}

DisplayInterfaceClass displayInterface;
