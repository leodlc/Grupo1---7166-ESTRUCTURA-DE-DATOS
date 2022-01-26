#pragma once
#include <string>
#include <functional>
#include <vector>
#include <map>
#include <memory>
#include <optional>

#ifndef __MENU_CLASSES_
#define __MENU_CLASSES_

class MenuOptionArgumentBase
{
public:
    virtual ~MenuOptionArgumentBase() = 0;
};

inline MenuOptionArgumentBase::~MenuOptionArgumentBase() {}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class MenuOptionArgument : public MenuOptionArgumentBase {
public:
    typedef T Type;
    explicit MenuOptionArgument(const Type& value);
    Type get_value();
    void set_value(Type value);
private:
    Type value;
};

template <typename T>
MenuOptionArgument<T>::MenuOptionArgument(const T& value): value(value) {}

template <typename T>
T MenuOptionArgument<T>::get_value() {
    return value;
}

template <typename T>
void MenuOptionArgument<T>::set_value(T value) {
    this->value = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

class MenuOptionArguments {
public:
    MenuOptionArguments();

    /**
     * @brief agrega un argumento con clave
     * @tparam T tipo del argumento
     * @param key 
     * @param argument 
    */
    template <typename T>
    void add(std::string key, MenuOptionArgument<T> *argument);

    /**
     * @brief Agrega un argumento con clave y un valor
     * @tparam T tipo de dato del argumento
     * @param key clave del argumento
     * @param value 
    */
    template <typename T>
    void add(std::string key, T value);

    /**
     * @brief obtiene un argumento por clave
     * @tparam T tipo de dato del argumento
     * @param key clave del argumento a obtener
     * @return 
    */
    template <typename T>
    T get(std::string key);

    /**
     * @brief define un argumento por clave y valor
     * @tparam T tipo de dato del argumento
     * @param key clave del argumento
     * @param value valor del argumento
    */
    template <typename T>
    void set(std::string key, T value);

    bool has(std::string key);
private:
    std::map<std::string, std::unique_ptr<MenuOptionArgumentBase>> *arguments;
};

inline MenuOptionArguments::MenuOptionArguments() {
    arguments = new std::map<std::string, std::unique_ptr<MenuOptionArgumentBase>>();
}

template <typename T>
void MenuOptionArguments::add(std::string key, MenuOptionArgument<T> *argument) {
    (*arguments)[key] = std::unique_ptr<MenuOptionArgumentBase>(argument);
}

template <typename T>
void MenuOptionArguments::add(std::string key, T value) {
    add(key, new MenuOptionArgument<T>(value));
}

template <typename T>
T MenuOptionArguments::get(std::string key) {
    auto arg = dynamic_cast<MenuOptionArgument<T>&>(*((*arguments)[key]));
    return arg.get_value();
}

template <typename T>
void MenuOptionArguments::set(std::string key, T value) {
    auto arg = dynamic_cast<MenuOptionArgument<T>&>(*((*arguments)[key]));
    arg.set_value(value);
}

inline bool MenuOptionArguments::has(std::string key) {
    return !(arguments->find(key) == arguments->end());
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef std::function<void(MenuOptionArguments args)> MenuOptionCallback;
typedef std::function<void()> MenuOptionCallback2;

class MenuOption {
public:
    MenuOption(std::string label, MenuOptionCallback callback);
    MenuOption(std::string label, MenuOptionCallback callback, bool wait_after_exec);
    MenuOption(std::string label, MenuOptionCallback callback, MenuOptionArguments arguments);
    MenuOption(std::string label, MenuOptionCallback callback, MenuOptionArguments arguments, bool wait_after_exec);

    MenuOption(std::string label, MenuOptionCallback2 callback);
    MenuOption(std::string label, MenuOptionCallback2 callback, bool wait_after_exec);
    MenuOption(std::string label, MenuOptionCallback2 callback, MenuOptionArguments arguments);
    MenuOption(std::string label, MenuOptionCallback2 callback, MenuOptionArguments arguments, bool wait_after_exec);

    bool visible();
    void visible(bool visible);
    void visible(std::function<bool()> visible_cb);

    /**
     * @brief obtiene la etiqueta de la opcion del menu
     * @return 
    */
    std::string get_label();

    /**
     * @brief ejecuta la accion de esta opcion del menu
    */
    void execute();

    /**
     * @brief verifica si esta opcion deberia esperar al usuario
     * @return true si deberia esperar, caso contrario devuelve false
    */
    bool should_wait();

    /**
     * @brief obtiene todos los argumentos de esta opcion
     * @return 
    */
    MenuOptionArguments &get_args();

private:
    std::string label;
    std::optional<MenuOptionCallback> callback;
    std::optional<MenuOptionCallback2> callback2;
    bool wait_after_exec = true;
    MenuOptionArguments arguments;
    std::function<bool()> visible_callback_ = []() { return true; };
};
