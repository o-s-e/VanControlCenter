#include "ConsoleForm.h"

void ConsoleFormClass::init(Genie &genie) {
    this->genie_ = &genie;
    text_.reserve(TEXT_BUFFER_SIZE);
    clear();
}

void ConsoleFormClass::update(Genie &genie) {
    //Nothing to do here
}

void ConsoleFormClass::print(String str) {
    text_.concat(str);
}

void ConsoleFormClass::println(String str) {
    addText(str + "\n");
}

void ConsoleFormClass::clear() {
    text_ = "";
}

void ConsoleFormClass::addText(String str) {
    int index = text_.indexOf("\n");
    //Remove first lines until there is enough space for the new text
    //Create a sort of scroll-up effect, like real shell
    if (str.length() < TEXT_BUFFER_SIZE) {
        while (text_.length() + str.length() >= TEXT_BUFFER_SIZE) {
            text_.remove(0, index == -1 ? text_.length() : index);
            index = text_.indexOf("\n");
        }
        text_.concat(str);
    }

    genie_->WriteStr(CONSOLE_STR, const_cast<char*>(text_.c_str()));
}

ConsoleFormClass consoleForm;