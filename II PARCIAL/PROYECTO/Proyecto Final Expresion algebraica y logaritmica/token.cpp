#include "token.h"

Token::Token(std::string value, TokenType type)
    : value_(value)
    , type_(type) {
    // ...
}

std::string Token::value() const {
    return value_;
}

void Token::value(std::string value) {
    value_ = value;
}

Token::TokenType Token::type() const {
    return type_;
}

void Token::type(TokenType type) {
    type_ = type;
}
