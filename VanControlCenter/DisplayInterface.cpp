#include "DisplayInterface.h"

void onEvent() {
    genieFrame evt;
    // Get the next pending event
    displayInterface.genie.DequeueEvent(&evt);

    // If current form is valid
    if (displayInterface.currentForm != NULL) {
        // Redirect the screen event on the current form invoking onEvent method
        displayInterface.currentForm->onEvent(displayInterface.genie, evt);
    }
}

void DisplayInterfaceClass::init() {

    // Init the serial comunication
    INIT_SERIAL(LCD_SERIAL, LCD_SERIAL_BAUD);
    while (!genie.Begin(LCD_SERIAL)) {
        Log.w(LCD_TAG) << F("Display not initialized") << Endl;
    }
    if (genie.online()) {
        Log.w(LCD_TAG) << F("Display online") << Endl;
    }
    genie.debug(LOG_SERIAL, 6);

    // Reset the LCD
    pinMode(LCD_RESET_PIN, OUTPUT);
    digitalWrite(LCD_RESET_PIN, LOW);
    delay(100);
    digitalWrite(LCD_RESET_PIN, HIGH);
    delay(4500);


    // Attach event handler method
    genie.AttachEventHandler(&onEvent);

    // set Brightness
    genie.WriteContrast(10);

    // Init forms
    consoleForm.init(genie);
    //debugForm.init(genie);
    temperatureControllerForm.init(genie);
    lightControllerForm.init(genie);
    generalSwitchForm.init(genie);
    currentForm = &consoleForm;

    // Refresh rate for the update method
    refreshTimer.setDuration(1000 / REFRESH_RATE).start();

    enabled = true;
}

void DisplayInterfaceClass::update() {

    // Update events
    genie.DoEvents();

    // If it's refresh time then update current form

    switch (genie.form()) {
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
        Log.w(LCD_TAG) << F("default form called: ") << genie.form() << Endl;
        break;
    }

    if (enabled && refreshTimer.hasFinished()) {
        if (currentForm != NULL) {
            currentForm->update(genie);
        }

        // If the refresh time is high disable the future updates. Probably the
        //screen froze or shut down

        if (refreshTimer.elapsedTime() > MAX_UPDATE_TIME) {
            enabled = false;
            Log.w(LCD_TAG) << F("Disabled LCD updates") << Endl;
        }


        refreshTimer.start();
    }
}

void DisplayInterfaceClass::setCurrentForm(LCDForm *currentForm) {

    const char *formTitles[] = { "ConsoleForm", "LightcontrollerForm", "TemperatureControllerForm", "GeneralForm", "DebugForm" };

    // If current form is not null invoke onExit method
    if (currentForm != NULL) {
        this->currentForm->onExit(genie);
    }

    this->currentForm = currentForm;

    if (currentForm != NULL) {
        // Activate next form
        genie.form(currentForm->getFormIndex());
        // Invoke new current form onEnter method
        currentForm->onEnter(genie);
        // Update new current form
        currentForm->update(genie);
        // Restart refresh timer
        refreshTimer.start();
    }
    Log.i(LCD_TAG) << F("Current display form set to: ") << formTitles[genie.form()] << Endl;
}

DisplayInterfaceClass displayInterface;