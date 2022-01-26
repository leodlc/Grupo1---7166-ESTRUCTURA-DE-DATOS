#define HAVE_CONFIG_H 1
#include "config.h"

#include <iostream>
#include <Magick++.h>
#include <Windows.h>

#include "options.h"
#include "term_info.h"
#include "asciifier.h"

int main(int argc, char** argv)
{
    im2a::Options options;

    options.source("imagen_1.jpeg");

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(handle, &csbi);

    _COORD coord;
    coord.X = 820;
    coord.Y = csbi.dwSize.Y;
    SetConsoleScreenBufferSize(handle, coord);

    im2a::TermInfo term_info;

    Magick::InitializeMagick(*argv);

    options.width(300);
    options.red_weight(0.4989f);
    options.green_weight(0.8866f);
    options.blue_weight(0.3145f);

    try {
        Magick::Image image("imagen_1.jpeg");
        im2a::Asciifier asciifier(&options, &term_info, &image);
        asciifier.asciify();
    } catch (std::exception const& err) {
        std::cout << err.what();
    }

    system("pause");
}