#include "menu.h"
#include <conio.h>
#include "console.h"

Menu::Menu() {
    conhandler = GetStdHandle(STD_OUTPUT_HANDLE);
}

Menu::Menu(std::string title) {
    this->title = title;
    conhandler = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Menu::add_option(MenuOption option) {
    options.push_back(option);
}

void Menu::remove_option(int index) {
    if (index < 0 || index > options.size()) {
        return;
    }

    std::vector<MenuOption>::iterator it = options.begin();
    std::advance(it, index);
    options.erase(it);
}

void Menu::display() {
    int selected = 1;

    if (options.empty()) {
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    WORD csbi_defaults;

    GetConsoleScreenBufferInfo(conhandler, &csbi);
    csbi_defaults = csbi.wAttributes;

    running = true;
        
    Console &console = Console::get();

    console.clear_area({ 0, 2, console.get_console_size().X, console.get_console_size().Y });
    console.set_cursor_position({0, 2});

    while (running) {
        int position = 1;

        console.clear_area({ 0, 2, console.get_console_size().X, console.get_console_size().Y });
        console.set_cursor_position({0, 2});

        std::vector<MenuOption> visible_options;

        std::cout << std::endl << title << std::endl << std::endl;

        for (MenuOption option : options) {
            if (option.visible()) {
                visible_options.push_back(option);
            }
        }

        for (MenuOption option : visible_options) {
            if (option.get_args().has("__index")) {
                option.get_args().set("__index", position - 1);
            } else {
                option.get_args().add("__index", position - 1);
            }

            if (position++ == selected) {
                SetConsoleTextAttribute (conhandler, 23);
            }

            std::cout << " " << option.get_label() << " " << std::endl;
            SetConsoleTextAttribute (conhandler, csbi_defaults);
        }

        int key;

        do {
#ifdef _MSC_VER
            key = _getch();

            if (key == 0) {
                key = _getch();
            }
#else
            key = getch();

            if (key == 0) {
                key = getch();
            }
#endif
        } while (key != KEY_UP && key != KEY_DOWN && key != KEY_ENTER);
        
        switch (key) {
            case KEY_UP: {
                selected--;

                if (selected < 1) {
                    selected = visible_options.size();
                }
            }
            break;
            case KEY_DOWN: {
                selected++;

                if (selected > visible_options.size()) {
                    selected = 1;
                }
            }
            break;
            case KEY_ENTER: {
                MenuOption option = visible_options.at(selected - 1);
                std::cout << std::endl << std::endl;
                option.execute();

                if (option.should_wait()) {
                    std::cout << std::endl << std::endl;
                    system("pause");
                }
            }
        }
    }
}

void Menu::stop() {
    running = false;
}

bool Menu::is_running() {
    return running;
}