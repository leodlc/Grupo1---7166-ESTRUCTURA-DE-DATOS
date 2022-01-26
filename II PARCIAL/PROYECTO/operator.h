#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <type_traits>

typedef std::function<double(std::vector<double> operands)> OperationCallback;

template< class T >
inline constexpr bool is_floating_point_val = std::is_floating_point<T>::value;

template<typename R, typename...> struct fst { typedef R type; };

class Operator {
public:
    enum class OperatorType {
        Regular,
        Function
    };

    enum class OperatorAssociativity {
        Left,
        Right,
        None
    };

    explicit Operator(
        std::string symbol,
        OperationCallback callback,
        OperatorType type,
        OperatorAssociativity associativity,
        size_t precedence,
        size_t operandsCount);

    std::string symbol() const;
    OperatorType type() const;
    bool matches(std::string symbol);
    OperatorAssociativity associativity() const;
    size_t precedence() const;
    size_t operandsCount() const;
    std::vector<std::string> aliases() const;

    template<typename... T, typename = std::enable_if_t<(... && is_floating_point_val<T>)>>
    double operate(T... args) {
        std::vector<double> operands{ { args... } };
        return operate(operands);
    }

    double operate(std::vector<double> operands) {
        return callback_(operands);
    }

    template<typename... T>
    Operator& alias(std::string arg, T... args) {
        std::vector<std::string> aliases{ {arg, args...} };

        for (std::string alias : aliases) {
            aliases_.push_back(alias);
        }

        return *this;
    }

private:
    std::string symbol_;
    OperatorType type_;
    size_t precedence_;
    size_t operandsCount_;
    OperatorAssociativity associativity_;
    OperationCallback callback_;
    std::vector<std::string> aliases_;
};
