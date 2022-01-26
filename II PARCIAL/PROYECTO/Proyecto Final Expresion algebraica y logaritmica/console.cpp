#include "console.h"

Console::Console() {}

Console& Console::get() {
    static Console instance;
    return instance;
}

void Console::print(std::string text, COORD coord) {
    CHAR_INFO *buffer = (CHAR_INFO*)calloc(text.size(), sizeof(CHAR_INFO));

	if (buffer == 0) {
		return;
	}

	COORD console_size = get_console_size();
	SMALL_RECT position = {
		coord.X, // left
		coord.Y, // top
		coord.X + text.length(), // right
		coord.Y + 1 // bottom
	};
	HANDLE console_handler = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD buffer_size = { text.size(), 1 };

	int i = 0;
	
	for (const char c : text) {
		buffer[i].Char.AsciiChar = c;
		buffer[i].Attributes = 15;
		i++;
	}

	WriteConsoleOutputA(console_handler, buffer, buffer_size, { 0, 0 }, &position);
	free(buffer);
}

void Console::clear_line(short y) {
    COORD console_size = get_console_size();

    if (y < 0 || y > console_size.Y) return;

    std::string empty(console_size.X, ' ');
    print(empty, { 0, y });
}

void Console::clear_screen() {
    COORD console_size = get_console_size();
    clear_area({ 0, 0, console_size.X, console_size.Y });
}

void Console::set_cursor_position(COORD coord) {
    HANDLE console_handler = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(console_handler, coord);
}

void Console::clear_area(SMALL_RECT area) {
    int x = area.Left;
    int y = area.Top;
    int width = area.Right - area.Left;
    int height = area.Bottom - area.Top;

    std::string spaces = std::string(width * height, ' ');
    CHAR_INFO *buffer = (CHAR_INFO*)calloc(spaces.size(), sizeof(CHAR_INFO));

	if (buffer == 0) {
		return;
	}

    SMALL_RECT position = {
		x, // left
		y, // top
		x + width, // right
		y + height // bottom
	};
    COORD buffer_size = { width, height };
    HANDLE console_handler = GetStdHandle(STD_OUTPUT_HANDLE);
    int i = 0;
	
	for (const char c : spaces) {
		buffer[i].Char.AsciiChar = c;
		buffer[i].Attributes = 15;
		i++;
	}

	WriteConsoleOutputA(console_handler, buffer, buffer_size, { 0, 0 }, &position);
	free(buffer);
}

COORD Console::get_console_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	COORD size = { columns, rows };

	return size;
}
