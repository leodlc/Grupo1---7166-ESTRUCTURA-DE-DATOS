#include "application.h"
#include "utils.h"
#include "menu.h"
#include "auth.h"

#include "notation_parser.h"

Application& Application::get() {
	static Application instance;
	return instance;
}

Application::Application() {}

int Application::initialize() {
	showAuthMenu();

	Menu menu("Menu");
	Marquee marquee("Universidad de las fuerzas armadas ESPE", Console::get().get_console_size().X - 3);
	
	menu.add_option(MenuOption("Realizar operacion", []() {
		std::string expression;

		do {
			std::cout << "ingrese una expresion: ";
			std::getline(std::cin, expression);
		} while (utils::trim(expression).empty());

		try {
			NotationParser parser(expression);
			double value = parser.calculate();

			std::cout << std::endl << "resultado: " << value << std::endl;
		} catch (std::exception ex) {
			std::cerr << ex.what() << std::endl;
		}
	}));
	menu.add_option(menu.exit_option());
	
	marquee.display();
	menu.display();

	return 0;
}

void Application::showAuthMenu() {
	Menu menu("Autentificacion");
	Auth auth;

	menu.add_option(MenuOption("Ingresar", [&]() {
		std::string username;
		std::string password;
		
		do {
			username = utils::read_line("Ingrese el nombre de usuario: ");
		} while (username.empty());

		do {
			password = utils::read_password("Ingrese una contrasenia: ");
		} while (password.empty());

		if (!auth.userExists(username)) {
			std::cout << std::endl << "[usuario no existe]" << std::endl << std::endl;
			system("pause");
			return;
		}

		if (auth.login(username, password)) {
			menu.stop();
		} else {
			std::cout << std::endl << "[Credenciales incorrectas]" << std::endl << std::endl;
			system("pause");
		}
	}, false));

	menu.add_option(MenuOption("Registrarse", [&]() {
		std::string username;
		std::string password;
		std::string password2;

		username = utils::read_line("Ingrese el nombre de usuario: ");

		do {
			password = utils::read_password("Ingrese una contrasenia: ");

			if (password.length() < 4) {
				std::cout << std::endl << "[La contrasenia debe tener 4 caracteres minimo]" << std::endl;
				continue;
			}

			password2 = utils::read_password("Ingrese una contrasenia: ");

			if (password != password2) {
				std::cout << std::endl << "[Las contrasenias no coinciden]" << std::endl;
				continue;
			}

			auth.registerUser(User(username, password));
			std::cout << std::endl << "[usuario registrado con exito]" << std::endl;

			break;
		} while (true);


	}, true));

	menu.add_option(MenuOption("Salir", []() {
		exit(0);
	}, false));

	menu.display();
}

bool Application::ingresar() {
	std::string username = utils::read_line("Ingrese el usuario: ");
	std::string password = utils::read_password("Ingrese la contrasenia: ");

	return false;
}