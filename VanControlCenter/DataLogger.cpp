#include "DataLogger.h"

void DataLoggerClass::init() {

    Configuration cfg;
    //Load cfg file

    if (cfg.loadFromFile(DL_CFG_FILE) == FILE_VALID) {
        updateCfg(cfg);
        createNewLogFile();
    }
    else {
        //Error on loading, searching a backup
        consoleForm.println(F("No valid Datalogger cfg file found"));
        Log.e(DL_TAG) << F("No valid Datalogger cfg file found") << Endl;

        if (cfg.loadFromBackup() == FILE_VALID) {
            updateCfg(cfg);
            createNewLogFile();
        }
        else {
            //No valid backup, load default values
            consoleForm.println(F("No valid backup found"));
            Log.e(DL_TAG) << F("No valid backup found") << Endl;

            consoleForm.println(F("Loading Datalogger default config"));
            Log.e(DL_TAG) << F("Loading Datalogger default config") << Endl;

            logFileName_ = LOG_FILE_FOLDER;
            logFileName_ += DEFAULT_LOG_NAME;
            createNewLogFile();

            logTimer_.setDuration(1000 / DEFAULT_SAMPLE_PER_S).start();
        }
    }
}


void DataLoggerClass::update() {
    //If it's time to log!
    if (channelsConfig.isValid() && logTimer_.hasFinished()) {
        if (SD.exists(logFileName_)) {
            //Open file
            File logFile = SD.open(logFileName_, O_WRITE);
            //Print them all
            if (logFile) {
                for (int i = 0; i < channelsConfig.getChannelCount(); i++) {
                    Channel * channel = channelsConfig.getChannelByIndex(i);
                    if (channelsBuffer.isValueUpdated(channel->getID())) {
                        logFile.print(
                            channelsBuffer.getValueAsString(channel->getID())
                        );
                    }
                    else {
                        logFile.print(' ');
                    }

                    logFile.print(',');
                }
                logFile.println();
                logFile.close();

                //Log.i(DL_TAG) << F("Logged") << Endl;
            }
            else {
                Log.e(DL_TAG) << F("Error opening log file!") << Endl;
            }
        }
        else {
            Log.e(DL_TAG) << F("Log file not found!") << Endl;
        }

        //Restart log timer
        logTimer_.start();
    }
}

void DataLoggerClass::updateCfg(Configuration& cfg) {
    //Build new log file name
    logFileName_ = LOG_FILE_FOLDER;
    logFileName_ += cfg[LOG_FILE_NAME].asString() + cfg[LOG_FILE_NUM].asString() + cfg[LOG_FILE_FORMAT].asString();

    //Set log-timer
    logTimer_.setDuration(1000 / cfg[SAMPLE_PER_SECOND].asInt()).start();

    //Save next log file name-index
    cfg.setValue(LOG_FILE_NUM, cfg[LOG_FILE_NUM].asInt() + 1);
    cfg.saveChanges();
}

void DataLoggerClass::createNewLogFile() {
    File logFile = SD.open(logFileName_, O_WRITE | O_CREAT);

    //Check if the file is opened correctly
    while (!logFile) {

        consoleForm.println(F("Error creating log file! Trying again..."));
        Log.e(DL_TAG) << F("Error creating log file! Trying again...") << Endl;

        delay(1000);
        logFile = SD.open(logFileName_, O_WRITE | O_CREAT);
    }

    for (int i = 0; i < channelsConfig.getChannelCount(); i++) {
        Channel * c = channelsConfig.getChannelByIndex(i);
        logFile.print(c->getName());
        logFile.print(F(" (0x"));
        logFile.print(c->getID(), HEX);
        logFile.print(F("),"));
    }
    logFile.println();
    logFile.close();
}

DataLoggerClass dataLogger;