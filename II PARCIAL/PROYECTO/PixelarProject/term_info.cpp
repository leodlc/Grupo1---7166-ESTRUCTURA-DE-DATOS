/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* term_info.cpp */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <stdlib.h>

/* these stupid macros shit all over the house */
#ifdef columns
#undef columns
#endif
#ifdef lines
#undef lines
#endif

#include "term_info.h"
#include <Windows.h>

im2a::TermInfo::TermInfo()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    /* fallback */
    _columns = columns;
    _lines = rows;
}

int im2a::TermInfo::columns() const
{
    return _columns;
}

int im2a::TermInfo::lines() const
{
    return _lines;
}
