#pragma once
#include <string>
#include <iostream>

class Token {
public:
    enum class TokenType {
        Operand,
        Operator,
        None
    };

    Token(std::string value, TokenType type);

    std::string value() const;
    void value(std::string value);

    TokenType type() const;
    void type(TokenType type);

    friend std::ostream& operator<<(std::ostream& os, Token const& t) {
        return os << t.value();
    }

private:
    std::string value_;
    TokenType type_;
};
