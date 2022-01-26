#pragma once
#include <string>
#include <thread>
#include <atomic>
#include "console.h"

struct MarqueeCharacter {
    char character;
    short x;
    short y;
};

class Marquee {
public:
    Marquee() = default;
    Marquee(std::string text, short limit);

    void text(std::string text);
    std::string text();
    void display();

private:
    std::string text_;
    short limit_;
    std::thread thread_;
    bool running_ = false;
    std::atomic_int step_ = 0;
    

    void process();
};