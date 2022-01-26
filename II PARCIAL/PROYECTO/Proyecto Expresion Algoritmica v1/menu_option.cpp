#include "menu_option.h"

MenuOption::MenuOption(std::string label, MenuOptionCallback callback) {
    this->label = label;
    this->callback = callback;
}

MenuOption::MenuOption(std::string label, MenuOptionCallback callback, bool wait_after_exec) {
    this->label = label;
    this->callback = callback;
    this->wait_after_exec = wait_after_exec;
}

MenuOption::MenuOption(std::string label, MenuOptionCallback callback, MenuOptionArguments arguments) {
    this->label = label;
    this->callback = callback;
    this->arguments = arguments;
}

MenuOption::MenuOption(std::string label, MenuOptionCallback callback, MenuOptionArguments arguments, bool wait_after_exec) {
    this->label = label;
    this->callback = callback;
    this->arguments = arguments;
    this->wait_after_exec = wait_after_exec;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MenuOption::MenuOption(std::string label, MenuOptionCallback2 callback) {
    this->label = label;
    this->callback2 = callback;
}

MenuOption::MenuOption(std::string label, MenuOptionCallback2 callback, bool wait_after_exec) {
    this->label = label;
    this->callback2 = callback;
    this->wait_after_exec = wait_after_exec;
}

MenuOption::MenuOption(std::string label, MenuOptionCallback2 callback, MenuOptionArguments arguments) {
    this->label = label;
    this->callback2 = callback;
    this->arguments = arguments;
}

MenuOption::MenuOption(std::string label, MenuOptionCallback2 callback, MenuOptionArguments arguments, bool wait_after_exec) {
    this->label = label;
    this->callback2 = callback;
    this->arguments = arguments;
    this->wait_after_exec = wait_after_exec;
}

bool MenuOption::visible() {
    return visible_callback_();
}

void MenuOption::visible(bool visible) {
    visible_callback_ = [=]() { return visible; };
}

void MenuOption::visible(std::function<bool()> callback) {
    visible_callback_ = callback;
}

std::string MenuOption::get_label() {
    return label;
}

void MenuOption::execute() {
    if (callback.has_value()) {
        callback.value()(arguments);
    } else {
        callback2.value()();
    }
}

bool MenuOption::should_wait() {
    return wait_after_exec;
}

MenuOptionArguments &MenuOption::get_args() {
    return arguments;
}