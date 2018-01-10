// ChannelsConfig.h

#ifndef _CHANNELSCONFIG_h
#define _CHANNELSCONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/**
* Class containing all the channels infos loaded by the cfg file
*/

#include "Channel.h"
#include "Configuration.h"
#include "Vector.h"

//Cfg file name
#define CHANNELS_CFG_FILE	"CHANNELS.CFG"

class ChannelsConfigClass {
public:
    //Init function to call in the setup
    boolean init();

    //Debug function to check if the parsing was successful
    void debug();

    //Get the channel by its id. Return NULL if a channel with can id equals to id is not present
    Channel* getChannelById(unsigned short id);

    //Get the channel by its array index
    Channel* getChannelByIndex(int index);

    //Find the channel array index with can id equals to id
    int getChannelIndex(unsigned short id);

    //Get the channels count
    int getChannelCount() { return channels_.getSize(); }

    //Return true if the cfg parsing was successful else return false
    boolean isValid() { return valid_; }

private:
    //True if the cfg parsing was successful else false
    boolean valid_{};

    //Vector containing all the loaded channells
    Vector<Channel*> channels_;
};

//Channels config instance
extern ChannelsConfigClass channelsConfig;

#endif
