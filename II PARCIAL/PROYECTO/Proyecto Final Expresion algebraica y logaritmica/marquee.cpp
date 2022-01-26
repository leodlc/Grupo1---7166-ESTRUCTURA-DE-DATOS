#include "marquee.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std::chrono_literals;

Marquee::Marquee(std::string text, short limit) : text_(text), limit_(limit) {
    thread_ = std::thread(&Marquee::process, this);
}

void Marquee::process() {
    Console& console_ = Console::get();
    COORD console_size = console_.get_console_size();
    std::string text;

    console_.clear_screen();

    if (text_.length() < limit_) {
        std::stringstream fixed;

        fixed
            << std::left
            << std::setw(limit_)
            << text_;

        text = fixed.str();
    }

    while (true) {
        std::string marquee_text;
        std::stringstream ss;
        int step = step_.load();

        if (step < limit_) {
            ss << std::right << std::setw(limit_) << text.substr(0, step);
        } else {
            ss << std::left << std::setw(limit_) << text.substr(step - limit_, limit_);

            if (step >= text_.length() + limit_) {
                step_.store(0);
            }
        }

        step_.store(step_.load() + 1); // step_++
        marquee_text = ss.str();
        console_.print("'" + marquee_text + "'", {0, 0});
        std::this_thread::sleep_for(90ms);
    }
}

void Marquee::display() {
    if (running_) return;
    running_ = true;
    thread_.detach();
}
