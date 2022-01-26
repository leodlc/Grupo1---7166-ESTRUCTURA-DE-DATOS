/** UNIVERSIDAD DE LAS FUERZAS ARMADAS "ESPE"
*			INGENIERIA SOFTWARE
*
*AUTORES: Luis Fernando Cueva
*Leonardo de la Cadena
*Steven Achig
*Eduardo Mortensen
*ARCHIVO: application.h
* */
#pragma once
#include "marquee.h"

class Application
{
public:
	Application(Application const&) = delete;
	Application& operator=(Application const&) = delete;
	static Application& get();
	int initialize();

private:
	Application();
	void showAuthMenu();
	bool ingresar();
};
