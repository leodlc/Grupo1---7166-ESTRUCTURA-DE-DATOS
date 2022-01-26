#pragma once
#include "menu_option.h"
#include <iostream>
#include <vector>
#include <windows.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 77
#define KEY_RIGHT 75
#define KEY_ENTER 13
#define KEY_ESCAPE 27

class Menu {
public:
    Menu();
    Menu(std::string title);

    /**
     * @brief agrega una opcion al menu
     * @param option 
     */
    void add_option(MenuOption option);

    /**
     * @brief remueve una opcion del menu
     * @param index indice de la opcion a eliminar
     */
    void remove_option(int index);

    /**
     * @brief muestra el menu
     */
    void display();

    /**
     * @brief detiene el menu
     */
    void stop();

    /**
     * @brief verifica si el menu esta detenido.
     * @return true si esta corriendo, caso contrario devuelve true
    */
    bool is_running();

    MenuOption exit_option(std::string label = "Salir", bool wait_after_exec = false) {
        return MenuOption(label, [&]() {
            stop();
        }, wait_after_exec);
    }

private:
    std::string title;
    std::vector<MenuOption> options;
    HANDLE conhandler;
    bool running = false;
};
