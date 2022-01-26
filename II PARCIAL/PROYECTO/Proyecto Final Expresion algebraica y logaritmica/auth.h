/** UNIVERSIDAD DE LAS FUERZAS ARMADAS "ESPE"
*			INGENIERIA SOFTWARE
*
*AUTORES: Luis Fernando Cueva
*Leonardo de la Cadena
*Steven Achig
*Eduardo Mortensen
*ARCHIVO: auth.h
* */
#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "utils.h"
#include "md5.h"

class User {
public:
	User(std::string username, std::string password) {
		username_ = username;
		password_ = password;
	}

	std::string username() {
		return username_;
	}

	std::string password() {
		return password_;
	}

private:
	std::string username_;
	std::string password_;
};

class Auth {
public:
	Auth() {
		loadUsers();
	}

	bool userExists(std::string username) {
		for (User user : users_) {
			std::string a = utils::strtolower(user.username());
			std::string b = utils::strtolower(username);

			if (a == b) {
				return true;
			}
		}

		return false;
	}

	bool login(std::string username, std::string password) {
		MD5 md5;

		for (User user : users_) {
			if (utils::strtolower(utils::strtolower(user.username())) == utils::trim(utils::strtolower(username))
				&& user.password() == md5(password)) {
				return true;
			}
		}

		return false;
	}

	void registerUser(User user) {
		std::ofstream archivo("users.txt", std::ios::app);
		MD5 md5;

		archivo << user.username() << "," << md5(user.password()) << std::endl;
		archivo.close();

		loadUsers();
	}

private:
	void loadUsers() {
		std::ifstream archivo("users.txt");
		std::string linea;

		users_.clear();

		while (std::getline(archivo, linea)) {
			std::istringstream ss(linea);
			std::string columna;
			std::vector<std::string> registros;

			while (std::getline(ss, columna, ',')) {
				registros.push_back(utils::trim(columna));
			}

			if (registros.size() != 2) {
				continue;
			}

			users_.push_back(User(registros.at(0), registros.at(1)));
		}

		archivo.close();
	}

	std::vector<User> users_;
};