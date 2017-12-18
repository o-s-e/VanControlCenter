// Shell.h

#ifndef SHELL_H
#define SHELL_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "ByteBuffer.h"
#include "ChannelsBuffer.h"
#include "ChannelsConfig.h"
#include "Logger.h"
#include <SD.h>

#define SHELL_TAG	F("SH")

#define SHELL_RX_BUFFER	32

class ShellClass;

struct ShellCommand {
    char cmdString[10];
    void (ShellClass::*cmdFun)(String&);
};

extern const PROGMEM ShellCommand cmdsList[];

#define CMDS_NUM	(sizeof(cmdsList) / sizeof(ShellCommand))

class ShellClass {
public:
    void init(Stream* serialPort);
    void update();

    //Cmds function
    void analogReadCmd(String&);
    void analogWriteCmd(String&);

    void channelListCmd(String&);
    void channelValueCmd(String&);
    void channelsValuesCmd(String&);

    void digitalReadCmd(String&);
    void digitalWriteCmd(String&);

    void sdMkDirCmd(String&);
    void sdOpenCmd(String&);
    void sdRmCmd(String&);
    void sdRmDirCmd(String&);
    void sdTreeCmd(String&);
    void cHbufferSet(String&);

private:
    ByteBuffer rxBuffer_;
    Stream* serialPort_;

    int findCmd(String& line, ShellCommand* cmd);
    String nextParam(String& params);

    void printSdTree(File dir, int indent);
};

extern ShellClass shell;

#endif
