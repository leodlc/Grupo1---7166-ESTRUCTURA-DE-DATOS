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
