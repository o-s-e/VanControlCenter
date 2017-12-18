#include "ChannelsConfig.h"
#include "ConsoleForm.h"

boolean ChannelsConfigClass::init() {
    Log << F("Load channels.cfg") << Endl;
    Channel *c;
    Configuration cfg;
    // Load cfg file
    if (cfg.loadFromFile(CHANNELS_CFG_FILE) == FILE_VALID) {
        Log << F("Channels.cfg is valid") << Endl;
        // Resize channel vector
        channels_.resize(cfg.getPropertyCount() / Channel::ATTR_COUNT);
        // Load cfg data
        for (int i = 0; i < cfg.getPropertyCount(); i += Channel::ATTR_COUNT) {
            c = new Channel;
            c->setId(cfg[i + Channel::CAN_ID].asInt());
            c->setName(cfg[i + Channel::NAME].asString());
            c->setSize(cfg[i + Channel::SIZE].asInt());
            c->setDataType(
                static_cast<Channel::DataTypes>(cfg[i + Channel::TYPE].asChar()));

            channels_.append(c);
        }

        valid_ = true;
    }
    else {
        // consoleForm.println(cfg.getErrorMsg());
        Log.e(CHBUF_TAG) << cfg.getErrorMsg() << Endl;
        valid_ = false;
    }

    return valid_;
}

void ChannelsConfigClass::debug() {
    Channel *c;

    Log << F("========== Channels loaded config: ==========") << Endl;
    Log << F("Channels: ") << channels_.getSize() << Endl;
    for (int i = 0; i < channels_.getSize(); i++) {
        c = channels_[i];
        Log << c->getID() << "  " << c->getName() << "  " << static_cast<char>(c->getDataType())
            << "  " << static_cast<int>(c->getSize()) << Endl;
    }
    Log << F("========================================") << Endl;;
}

Channel *ChannelsConfigClass::getChannelById(unsigned short id) {
    return getChannelByIndex(getChannelIndex(id));
}

int ChannelsConfigClass::getChannelIndex(unsigned short id) {
    // Binary search channel's index by canID
    int s = 0, d = channels_.getSize() - 1;
    int p;

    while (s <= d) {
        p = (s + d) / 2;

        if (channels_[p]->getID() == id) {
            return p;
        }
        else if (id > channels_[p]->getID()) {
            s = p + 1;
        }
        else {
            d = p - 1;
        }
    }

    return -1;
}

Channel *ChannelsConfigClass::getChannelByIndex(int index) {
    if (index != -1 && index < channels_.getSize()) {
        return channels_[index];
    }
    return NULL;
}

ChannelsConfigClass channelsConfig;