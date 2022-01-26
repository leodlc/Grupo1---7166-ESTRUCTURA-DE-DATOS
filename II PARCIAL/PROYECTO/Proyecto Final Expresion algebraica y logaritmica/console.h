/** UNIVERSIDAD DE LAS FUERZAS ARMADAS "ESPE"
*			INGENIERIA SOFTWARE
*
*AUTORES: Luis Fernando Cueva
*Leonardo de la Cadena
*Steven Achig
*Eduardo Mortensen
*ARCHIVO: console.h
* */
#pragma once
#include <string>
#include <Windows.h>

class Console
{
public:
    Console(Console const&) = delete;
	Console& operator=(Console const&) = delete;
	static Console& get();

    void print(std::string text, COORD coord);
    void clear_line(short y);
    void clear_screen();
    void clear_area(SMALL_RECT area);
    void set_cursor_position(COORD coord);
    COORD get_console_size();

private:
    Console();
};