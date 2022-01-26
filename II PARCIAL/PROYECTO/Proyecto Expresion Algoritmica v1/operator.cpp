#include "operator.h"
#include <algorithm>

Operator::Operator(std::string symbol,
                   OperationCallback callback,
                   OperatorType type,
                    OperatorAssociativity associativity,
                   size_t precedence,
                   size_t operandsCount)
    : symbol_(symbol)
    , type_(type)
    , associativity_(associativity)
    , precedence_(precedence)
    , operandsCount_(operandsCount)
    , callback_(callback) {}

std::string Operator::symbol() const {
    return symbol_;
}

Operator::OperatorType Operator::type() const {
    return type_;
}

Operator::OperatorAssociativity Operator::associativity() const {
    return associativity_;
}

size_t Operator::precedence() const {
    return precedence_;
}

size_t Operator::operandsCount() const {
    return operandsCount_;
}

std::vector<std::string> Operator::aliases() const
{
    return aliases_;
}

bool Operator::matches(std::string symbol) {
    return (symbol_ == symbol 
        || (!aliases_.empty() && std::find(aliases_.begin(), aliases_.end(), symbol) != aliases_.end()));
}