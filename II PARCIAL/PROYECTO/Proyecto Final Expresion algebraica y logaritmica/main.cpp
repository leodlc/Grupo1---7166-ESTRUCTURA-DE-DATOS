/** UNIVERSIDAD DE LAS FUERZAS ARMADAS "ESPE"
*			INGENIERIA SOFTWARE
*
*AUTORES: Luis Fernando Cueva
*Leonardo de la Cadena
*Steven Achig
*Eduardo Mortensen
*ARCHIVO: main.cpp
* */
#include "Application.h"

int main()
{
    Application& app = Application::get();
    return app.initialize();
}
