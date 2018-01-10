#include "Shell.h"

//MUST be in alphabetical order
const PROGMEM ShellCommand cmdsList[] = {
    //analog pins cmds
    {"ar", &ShellClass::analogReadCmd}, //analogRead							ar	<pin>
    {"aw", &ShellClass::analogWriteCmd}, //analogWrite							aw	<pin>	<value>

    //channels cmds
    {"chbset", &ShellClass::cHbufferSet},
    // set channelbuffer value                       chbset <type> <channel> <value>
    {"chlist", &ShellClass::channelListCmd}, //list all loaded channels				chlist
    {"chvalue", &ShellClass::channelValueCmd}, //list a specific channel last value	chvalue <id>
    {"chvalues", &ShellClass::channelsValuesCmd}, //list all last channel values			chvalues

    //digital pins cmds
    {"dr", &ShellClass::digitalReadCmd}, //digitalRead							dr	<pin>
    {"dw", &ShellClass::digitalWriteCmd}, //digitalWrite							dw	<pin>	<value>

    //SD cmds
    {"sdmkdir", &ShellClass::sdMkDirCmd}, //make a dir							sdmkdir <path>
    {"sdopen", &ShellClass::sdOpenCmd}, //read all file's content				sdopen	<path>
    {"sdrm", &ShellClass::sdRmCmd}, //remove a file							sdrm	<path>
    {"sdrmdir", &ShellClass::sdRmDirCmd}, //remove a dir							sdrmdir	<path>
    {"sdtree", &ShellClass::sdTreeCmd} //print all files and dirs				sdtree


};

void ShellClass::init(Stream* serialPort) {
    this->serialPort_ = serialPort;
    this->rxBuffer_.resize(SHELL_RX_BUFFER);
    Log.i(SHELL_TAG) << F("Shell initialized") << Endl;
}

void ShellClass::update() {

    int index;
    String line;
    ShellCommand cmd{};

    if (serialPort_->available()) {
        while (serialPort_->available() && rxBuffer_.getSize() < rxBuffer_.getCapacity()) {
            rxBuffer_.append(serialPort_->read());
        }

        index = rxBuffer_.indexOf('\n');
        if (index != -1) {
            line = rxBuffer_.toString(0, index);
            line.trim();

            if (findCmd(line, &cmd) != -1) {
                (this ->* cmd.cmdFun)(line);
                Log.e(SHELL_TAG) << ("Cmd executed") << Endl;
            }
            else {
                Log.e(SHELL_TAG) << F("Command not found") << Endl;
            }

            rxBuffer_.clear();
        }
    }
}

//Cmds
//A
void ShellClass::analogReadCmd(String& params) {
    int pin = A0 + nextParam(params).toInt();
    uint32_t attr = g_APinDescription[pin].ulPinAttribute;

    if ((attr & PIN_ATTR_ANALOG) != PIN_ATTR_ANALOG) {
        Log.e(SHELL_TAG) << F("Pin number A") << pin << F(" is not a valid analog I/O pin for this board") << Endl;
    }
    else {
        pinMode(pin, INPUT);
        Log.i(SHELL_TAG) << F("Readed ") << analogRead(pin) << Endl;
    }
}

void ShellClass::analogWriteCmd(String& params) {
    int pin = A0 + nextParam(params).toInt();
    int value = nextParam(params).toInt();

    uint32_t attr = g_APinDescription[pin].ulPinAttribute;

    if ((attr & PIN_ATTR_ANALOG) != PIN_ATTR_ANALOG && (attr & PIN_ATTR_PWM) != PIN_ATTR_PWM) {
        Log.e(SHELL_TAG) << F("Pin number A") << pin << F(" is not a valid analog I/O pin for this board") << Endl;
    }
    else {
        pinMode(pin, OUTPUT);
        analogWrite(pin, value);
        Log.i(SHELL_TAG) << F("OK") << Endl;
    }
}

//C
void ShellClass::channelListCmd(String& params) {
    Channel* c;

    Log.i(SHELL_TAG) << F("Channel number: ") << channelsConfig.getChannelCount() << Endl;
    Log.i(SHELL_TAG) << F("ID\tName\tSize\tType") << Endl;

    for (int i = 0; i < channelsConfig.getChannelCount(); i++) {
        c = channelsConfig.getChannelByIndex(i);
        Log.i(SHELL_TAG) << "  " << c->getID() << "\t" << c->getName() << "\t" << c->getSize() << "\t" << static_cast<
            char>(c->getDataType()) << Endl;
    }
}

void ShellClass::channelValueCmd(String& params) {
    int id = nextParam(params).toInt();
    Channel* c = channelsConfig.getChannelById(id);

    if (c == NULL) {
        Log.e(SHELL_TAG) << F("Channel not found") << Endl;
    }
    else {
        Log.i(SHELL_TAG) << c->getID() << "\t" << channelsBuffer.getValueAsString(id) << "\t" << channelsBuffer.
            getValueAsByteArray(id).toHexString() << Endl;
    }
}

void ShellClass::channelsValuesCmd(String& params) {
    Channel* c;
    for (int i = 0; i < channelsConfig.getChannelCount(); i++) {
        c = channelsConfig.getChannelByIndex(i);
        int id = c->getID();
        Log.i(SHELL_TAG) << c->getID() << "\t" << channelsBuffer.getValueAsString(id) << "\t" << channelsBuffer.
            getValueAsByteArray(id).toHexString() << Endl;
    }
}

void ShellClass::digitalReadCmd(String& params) {
    int pin = nextParam(params).toInt();

    if (g_APinDescription[pin].ulPinType == PIO_NOT_A_PIN) {
        Log.e(SHELL_TAG) << F("Pin number ") << pin << F(" is not a valid digital I/O pin for this board") << Endl;
    }
    else {
        pinMode(pin, INPUT);
        Log.i(SHELL_TAG) << F("Readed ") << digitalRead(pin) << Endl;
    }
}

void ShellClass::digitalWriteCmd(String& params) {
    int pin = nextParam(params).toInt();
    int level = nextParam(params).toInt();

    if (g_APinDescription[pin].ulPinType == PIO_NOT_A_PIN) {
        Log.e(SHELL_TAG) << F("Pin number ") << pin << F(" is not a valid digital I/O pin for this board") << Endl;
    }
    else {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, level);
        Log.i(SHELL_TAG) << F("OK") << Endl;
    }
}

//S
void ShellClass::sdMkDirCmd(String& params) {
    String path = nextParam(params);

    if (SD.mkdir(path)) {
        Log.i(SHELL_TAG) << F("OK") << Endl;
    }
    else {
        Log.e(SHELL_TAG) << F("Error creating new directory: is SD inserted?") << Endl;
    }
}

void ShellClass::sdOpenCmd(String& params) {
    String path = nextParam(params);

    if (SD.exists(path)) {
        File f = SD.open(path);
        f.setTimeout(100);
        if (f) {
            while (f.available()) {
                Log.i(SHELL_TAG) << f.readStringUntil('\n') << Endl;
            }
            f.close();
        }
        else {
            Log.e(SHELL_TAG) << F("Error opening file") << Endl;
        }
    }
    else {
        Log.e(SHELL_TAG) << F("File not found") << Endl;
    }
}

void ShellClass::sdRmCmd(String& params) {
    String path = nextParam(params);

    if (SD.exists(path)) {
        if (SD.remove(path)) {
            Log.i(SHELL_TAG) << F("OK") << Endl;
        }
        else {
            Log.e(SHELL_TAG) << F("Error deleting file") << Endl;
        }
    }
    else {
        Log.e(SHELL_TAG) << F("File not found") << Endl;
    }
}

void ShellClass::sdRmDirCmd(String& params) {
    String path = nextParam(params);

    if (SD.exists(path)) {
        if (SD.rmdir(path)) {
            Log.i(SHELL_TAG) << F("OK") << Endl;
        }
        else {
            Log.e(SHELL_TAG) << F("Error deleting directory") << Endl;
        }
    }
    else {
        Log.e(SHELL_TAG) << F("Directory not found") << Endl;
    }
}

void ShellClass::sdTreeCmd(String&) {
    File root = SD.open("/");
    root.rewindDirectory();

    if (!root) {
        Log.e(SHELL_TAG) << F("Error opening SD root: is SD inserted?") << Endl;
    }
    else {
        printSdTree(root, 0);
    }
}

void ShellClass::cHbufferSet(String& params) {
    String type = nextParam(params);
    int channel = nextParam(params).toInt();

    if (type == "i") {

        unsigned int value = nextParam(params).toInt();
        channelsBuffer.setValue<unsigned int>(channel, value);
    }
    else if (type == "d") {

        double value = nextParam(params).toDouble();
        channelsBuffer.setValue<double>(channel, value);
    }
    else if (type == "u") {

        uint_fast8_t value = nextParam(params).toDouble();
        channelsBuffer.setValue<uint_fast8_t>(channel, value);
    }
    else {
        Log.e(SHELL_TAG) << F("Set first param to i for int or d for double") << Endl;
    }

}

void ShellClass::printSdTree(File dir, int indent) {
    File entry = dir.openNextFile();

    while (entry) {
        Log.i(SHELL_TAG) << Log.repeat('\t', indent) << entry.name();

        if (entry.isDirectory()) {
            Log << '/' << Endl;
            printSdTree(entry, indent + 1);
        }
        else {
            Log << '\t' << entry.size() << Endl;
        }

        entry.close();
        entry = dir.openNextFile();
    }
}

//Cmd utils

int ShellClass::findCmd(String& line, ShellCommand* cmd) {
    //Binary search channel's index by canID
    int s = 0, d = CMDS_NUM - 1;
    int p, cmp;

    int index = line.indexOf(' ');
    String cmdLine;

    //Remove \n and \r
    line.remove(line.indexOf('\n'), 1);
    line.remove(line.indexOf('\r'), 1);

    //Get the cmd
    cmdLine = line.substring(0, index == -1 ? line.length() : index);

    //Remove cmd string and leave only the params
    line.remove(0, index + 1);

    while (s <= d) {
        p = (s + d) / 2;

        //Load cmd from flash
        mempcpy(cmd, &cmdsList[p], sizeof(ShellCommand));
        //Compare loaded cmd string and searched cmd line
        cmp = strcmp(cmd->cmdString, cmdLine.c_str());

        if (cmp == 0) {
            //Log << Hex << Log.array<byte>((byte*)line.c_str(), line.length()) << Endl;

            return p;
        }
        if (cmp < 0) {
            s = p + 1;
        }
        else {
            d = p - 1;
        }
    }

    cmd = NULL;
    return -1;
}

String ShellClass::nextParam(String& params) {
    int index = params.indexOf(' ');
    String p;

    if (index != -1) {
        p = params.substring(0, index);
        params.remove(0, index + 1);
        return p;
    }
    //if not found the params string is empty or contains the last param
    return params;
}


ShellClass shell;
