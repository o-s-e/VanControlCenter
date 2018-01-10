#include "LCDStringList.h"

void LcdStringList::init(byte widgetIndex, int maxSize, Genie* parent) {
    this->widgetIndex_ = widgetIndex; //LCD widget index
    this->currentElement_ = -1; //No elements
    this->elementCount_ = 0; //No elements
    this->buffer_.reserve(maxSize); //Reserve buffer
    this->parent_ = parent;
}

void LcdStringList::addElement(const String& elem) {
    //If no elements set the first selected
    if (elementCount_ == 0) {
        currentElement_ = 0;
        buffer_.concat(SELECT_LINE_CHAR);
    }
    else {
        buffer_.concat(' ');
    }

    //Add element
    buffer_.concat(elem);
    buffer_.concat('\n');
    elementCount_++;
}

void LcdStringList::clear() {
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

    //Reset
    currentElement_ = -1;
    elementCount_ = 0;
}

void LcdStringList::down() {
    //If there are almost two elements and if the last element is not the current one
    if (elementCount_ > 1 && currentElement_ < elementCount_ - 1) {
        //Search for the current line and remove '>'
        int currentLine = buffer_.indexOf(SELECT_LINE_CHAR);
        buffer_.setCharAt(currentLine, ' ');

        //Skip to next line
        currentLine = buffer_.indexOf('\n', currentLine);
        //Go down
        buffer_.setCharAt(currentLine + 1, SELECT_LINE_CHAR);
        //Update
        currentElement_++;
        repaint();
    }
}

void LcdStringList::removeElement(int index) {
    //TODO: Implement if necessary
}

void LcdStringList::repaint() {
    parent_->WriteStr(int(widgetIndex_), const_cast<char*>(buffer_.c_str()));
}

void LcdStringList::up() {
    int index = 0;
    //If there are almost two elements and if the first element is not the current one
    if (elementCount_ > 1 && currentElement_ != 0) {
        int current = buffer_.indexOf(SELECT_LINE_CHAR);
        buffer_.setCharAt(current, ' ');

        for (int i = 0; i < currentElement_ - 1; i++) {
            index = buffer_.indexOf('\n', index + 1);
        }

        if (index > 0) {
            buffer_.setCharAt(index + 1, SELECT_LINE_CHAR);
            currentElement_--;
        }
        else {
            currentElement_ = 0;
            buffer_.setCharAt(0, SELECT_LINE_CHAR);
        }

        //Update
        repaint();
    }
}

int LcdStringList::getCurrentElement() {
    return currentElement_;
}

int LcdStringList::getWidgetIndex() {
    return widgetIndex_;
}

int LcdStringList::getElementCount() {
    return elementCount_;
}
