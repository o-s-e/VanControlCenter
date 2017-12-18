#include "LCDStringMsg.h"

void LcdStringMsg::init(byte widgetIndex, Genie* parent) {
    this->widgetIndex_ = widgetIndex;
    this->parent_ = parent;
}

void LcdStringMsg::clear() {
    //Replace all char except \n with with space
    for (uint16_t i = 0; i < buffer_.length(); i++) {
        if (buffer_.charAt(i) != '\n') {
            buffer_.setCharAt(i, ' ');
        }
    }

    //Update screen
    repaint();

    //Clear the buffer
    buffer_.remove(0, buffer_.length());
}

const String& LcdStringMsg::getMessage() {
    return buffer_;
}

void LcdStringMsg::repaint() {
    parent_->WriteStr(widgetIndex_, const_cast<char*>(buffer_.c_str()));
}

void LcdStringMsg::setMessage(String str) {
    clear();
    buffer_ += str;
    repaint();
}

void LcdStringMsg::setMessage(const char* str) {
    clear();
    buffer_ += str;
    repaint();
}
