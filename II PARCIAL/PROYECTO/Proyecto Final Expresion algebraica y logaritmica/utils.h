#pragma once
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <conio.h>
#include <windows.h>

#ifndef _MSC_VER
#define _getch getch
#endif

namespace utils
{
    inline static double to_degrees(double radians) {
        return (radians * 180) / M_PI;
    }

    inline static double to_radians(double degrees) {
        return (degrees * M_PI) / 180;
    }

    /**
     * @brief verifica si una cadena es numerica
     * @param str cadena a ser evaluada
     * @return true si es numerica, caso contrario devuelve false
     */
    static inline int is_numeric(std::string str) {
        return !str.empty() && std::find_if(str.begin(),
            str.end(), [](unsigned char c) {
                return !std::isdigit(c);
            }) == str.end();
    }

    inline static std::string trim(const std::string& str) {
        std::string trimmed(str);
        trimmed = trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
        trimmed = trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));

        return trimmed;
    }

    inline static std::string strtolower(const std::string& str) {
        std::string output(str);

        std::transform(output.begin(), output.end(), output.begin(),
            [](unsigned char c) { return std::tolower(c); });

        return output;
    }

    inline static std::string read_line(const std::string label) {
        std::string input;
        std::cout << label;
        std::getline(std::cin, input);

        return input;
    }

    inline static bool confirm(const std::string question) {
        std::string input;
        bool answer;

        do {
            input = read_line(question + " (s/n): ");

            if (input.empty()) {
                continue;
            }

            if (input.length() != 1) {
                continue;
            }

            char c = std::tolower(input.at(0));

            if (c != 's' && c != 'n') {
                continue;
            }

            answer = (c == 's');

            break;
        } while (true);

        return answer;
    }

    inline static std::string read_password(std::string label) {
        const char BACKSPACE = 8;
        const char RETURN = 13;

        std::string password;
        int ch = 0;

        std::cout << label;

        while (true) {
            ch = _getch();

            if (ch == 0x00) {
                ch = _getch();
            }

            if (ch == RETURN) {
                break;
            }

            if (ch == BACKSPACE) {
                if (password.length() != 0) {
                    std::cout << "\b \b";
                    password.resize(password.length() - 1);
                } else {
                    Beep(523, 100);
                }
            } else {
                password += ch;
                std::cout << '*';
            }
        }

        std::cout << std::endl;
        
        return password;
    }
}
