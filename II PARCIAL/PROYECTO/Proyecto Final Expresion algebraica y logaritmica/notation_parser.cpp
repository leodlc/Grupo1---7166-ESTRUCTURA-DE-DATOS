#include "notation_parser.h"
#include "stack.h"
#include "token.h"
#include "utils.h"
#include "binary_tree.h"

#include <iomanip>
#include <cctype>
#include <cmath>
#include <optional>
#include <iostream>
#include <stack>
#include <queue>
#include <map>

NotationParser::NotationParser(std::string expression)
    : expression_(expression) {
    // registrar operadores por defecto
    registerDefaultOperators();
}

double NotationParser::calculate() {
    return evaluate(parseTree().root());
}

double NotationParser::evaluate(TreeNode<Token>* root) {
    auto token = root->value();

    if (token.type() == Token::TokenType::Operator) {
        auto operator_ = getOperator(token.value()).value();

        if (operator_.type() == Operator::OperatorType::Function) {
            return operator_.operate(evaluate(root->right()));
        } else {
            return operator_.operate(evaluate(root->right()), evaluate(root->left()));
        }
    }

    std::string value = token.value();

    if (value[0] == '-') {
        return std::stod(value.substr(1)) * -1;
    } else {
        return std::stod(value);
    }
}

BinaryTree<Token> NotationParser::parseTree() {
    if (!hasValidParenthesis()) {
        throw std::runtime_error("Invalid expression: mismatched parenthesis");
    }

    std::queue<Token> output;
    std::stack<Token> operators;
    Token::TokenType last_type = Token::TokenType::None;
    int index = 0;

    while (index < expression_.length()) {
        int steps = 1;
        char character = expression_[index];
        std::string token;
        std::optional<Operator> _operator = std::nullopt;
        bool negative = false;

        // ignoramos los espacios
        if (isspace(character)) {
            index++;
            continue;
        }

        if (character == '#') {
            output.push(Token(std::to_string(M_PI), Token::TokenType::Operand));
            index++;
            continue;
        }

        for (Operator op : operators_) {
            std::string symbol = op.symbol();

            if (op.matches(expression_.substr(index, symbol.length()))) {
                _operator = op;
                break;
            }

            std::vector<std::string> aliases = op.aliases();

            for (std::string alias : aliases) {
                if (alias == expression_.substr(index, alias.length())) {
                    _operator = op;
                    break;
                }
            }

            if (_operator) {
                break;
            }
        }

        if (character == '-' && (last_type == Token::TokenType::Operator 
            || last_type == Token::TokenType::None) && !expression_.substr(index).empty()) {
            _operator = std::nullopt;
            negative = true;
            character = expression_[index + 1];
            index++;
        }

        if (_operator) {
            auto currentOperator = _operator.value();
            std::string symbol = currentOperator.symbol();

            if (symbol != expression_.substr(index, symbol.length())) {
                std::vector<std::string> aliases = currentOperator.aliases();

                for (std::string alias : aliases) {
                    if (alias == expression_.substr(index, alias.length())) {
                        symbol = alias;
                        break;
                    }
                }
            }

            if (currentOperator.type() == Operator::OperatorType::Regular) {
                while (!operators.empty()
                    && operators.top().value() != "("
                    && ((getOperator(operators.top().value()).value().precedence() >
                        currentOperator.precedence())
                        || (getOperator(operators.top().value()).value().associativity()
                            == Operator::OperatorAssociativity::Left
                            && (getOperator(operators.top().value()).value().precedence()
                                == currentOperator.precedence())))) {
                    output.push(operators.top());
                    operators.pop();
                }
            }

            operators.push(Token(symbol, Token::TokenType::Operator));
            steps = symbol.length();
            last_type = Token::TokenType::Operator;
        } else if (character == '(') {
            operators.push(Token("(", Token::TokenType::Operator));
        } else if (character == ')') {
            if (operators.empty()) {
                throw std::runtime_error("Invalid expression: mismatched parenthesis");
            }

            while (operators.top().value() != "(") {
                output.push(operators.top());
                operators.pop();
            }

            if (operators.empty() || operators.top().value() != "(") {
                throw std::runtime_error("Invalid expression: mismatched parenthesis");
            }

            operators.pop();

            if (!operators.empty()) {
                bool isFunction = false;

                for (Operator op : operators_) {
                    std::string symbol = op.symbol();

                    if (op.type() == Operator::OperatorType::Function && op.matches(operators.top().value())) {
                        isFunction = true;
                    }
                }

                if (isFunction) {
                    output.push(operators.top());
                    operators.pop();
                }
            }
        } else if (::isdigit(character)) { // operand
            char current;
            std::string value;
            int subindex = index;

            do {
                current = expression_.at(subindex);

                if (isdigit(current) || current == '.') {
                    value.push_back(current);
                }

                subindex++;
            } while ((isdigit(current) || current == '.') && subindex < expression_.size());

            if (negative) {
                value = "-" + value;
            }

            output.push(Token(value, Token::TokenType::Operand));
            steps = value.size() - (negative ? 1 : 0);
            last_type = Token::TokenType::Operand;
        } else {
            throw std::runtime_error(
                "Invalid expression: unexpected character '" + std::string(1, character) + "'");
        }

        index += steps;
    }

    while (!operators.empty()) {
        Token token = operators.top();

        if (token.value() == "(") {
            throw std::runtime_error(
                "Invalid expression: mismatched parenthesis");
        }

        output.push(token);
        operators.pop();
    }

    std::stack<double> stack;
    std::stack<TreeNode<Token>*> nodes;

    while (!output.empty()) {
        auto token = output.front();

        if (token.type() == Token::TokenType::Operand) {
            std::string value = token.value();

            if (value[0] == '-') {
                stack.push(std::stod(token.value().substr(1)) * -1);
            } else {
                stack.push(std::stod(token.value()));
            }
            
            nodes.push(new TreeNode<Token>(token));
        } else {
            if (!getOperator(token.value())) {
                throw std::runtime_error("Invalid expression");
            }

            auto op = getOperator(token.value()).value();

            std::vector<double> operands;

            for (int i = 0; i < op.operandsCount(); i++) {
                if (stack.empty()) {
                    throw std::runtime_error(std::string("Invalid expression"));
                }

                operands.push_back(stack.top());
                stack.pop();
            }

            stack.push(op.operate(operands));

            TreeNode<Token>* root = new TreeNode(token);

            root->right(nodes.top());
            nodes.pop();

            if (op.type() != Operator::OperatorType::Function) {
                root->left(nodes.top());
                nodes.pop();
            }

            nodes.push(root);
        }

        //std::cout << token << " ";
        output.pop();
    }

    auto top = nodes.top();
    return BinaryTree<Token>(top);
}

std::optional<Operator> NotationParser::getOperator(std::string symbol) {
    for (Operator op : operators_) {
        if (op.matches(symbol)) {
            return { op };
        }
    }

    return std::nullopt;
}

bool NotationParser::hasValidParenthesis() {
    Stack<char> stack;

    for (char character : expression_) {
        if (character == '(') { // se encontro un parentesis de apertura
            stack.push(character);
        } else if (character == ')') { // se encontro un parentesis de cierre
            if (stack.empty()) {
                return false;
            } else {
                stack.pop();
            }
        }
    }

    return stack.empty();
}

void NotationParser::registerDefaultOperators() {
    operators_.push_back(Operator("<=", [](std::vector<double> operands) {
        return operands.at(1) <= operands.at(0);
        }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 0, 2));

    operators_.push_back(Operator(">=", [](std::vector<double> operands) {
        return operands.at(1) >= operands.at(0);
        }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 0, 2));

    operators_.push_back(Operator(">", [](std::vector<double> operands) {
        return (operands.at(1) > operands.at(0));
        }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 0, 2));

    operators_.push_back(Operator("<", [](std::vector<double> operands) {
        return operands.at(1) < operands.at(0);
        }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 0, 2));

    operators_.push_back(Operator("==", [](std::vector<double> operands) {
        return operands.at(1) == operands.at(0);
        }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 0, 2));

    operators_.push_back(Operator("!=", [](std::vector<double> operands) {
        return operands.at(1) != operands.at(0);
        }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 0, 2));

    operators_.push_back(Operator("+", [](std::vector<double> operands) {
        return operands.at(1) + operands.at(0);
    }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 1, 2));

    operators_.push_back(Operator("-", [](std::vector<double> operands) {
        return operands.at(1) - operands.at(0);
    }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 2, 2));

    operators_.push_back(Operator("*", [](std::vector<double> operands) {
        return operands.at(1) * operands.at(0);
    }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 3, 2));

    operators_.push_back(Operator("/", [](std::vector<double> operands) {
        return operands.at(1) / operands.at(0);
    }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Left, 3, 2));

    operators_.push_back(Operator("^", [](std::vector<double> operands) {
        return std::pow(operands.at(1), operands.at(0));
    }, Operator::OperatorType::Regular, Operator::OperatorAssociativity::Right, 4, 2));

    operators_.push_back(Operator("sqrt", [](std::vector<double> operands) {
        return std::sqrt(operands.at(0));
    }, Operator::OperatorType::Function, Operator::OperatorAssociativity::None, 5, 1).alias("raiz"));

    operators_.push_back(Operator("sin", [](std::vector<double> operands) {
        return std::sin(utils::to_radians(operands.at(0)));
    }, Operator::OperatorType::Function, Operator::OperatorAssociativity::None, 5, 1).alias("sen"));

    operators_.push_back(Operator("cos", [](std::vector<double> operands) {
        return std::cos(utils::to_radians(operands.at(0)));
    }, Operator::OperatorType::Function, Operator::OperatorAssociativity::None, 5, 1));

    operators_.push_back(Operator("tan", [](std::vector<double> operands) {
        return std::tan(utils::to_radians(operands.at(0)));
    }, Operator::OperatorType::Function, Operator::OperatorAssociativity::None, 5, 1));

    operators_.push_back(Operator("atan", [](std::vector<double> operands) {
        return std::atan(utils::to_radians(operands.at(0)));
    }, Operator::OperatorType::Function, Operator::OperatorAssociativity::None, 5, 1).alias("arctan"));

    operators_.push_back(Operator("acos", [](std::vector<double> operands) {
        return std::acos(utils::to_radians(operands.at(0)));
    }, Operator::OperatorType::Function, Operator::OperatorAssociativity::None, 5, 1).alias("arcos"));

    operators_.push_back(Operator("asin", [](std::vector<double> operands) {
        return std::asin(utils::to_radians(operands.at(0)));
    }, Operator::OperatorType::Function, Operator::OperatorAssociativity::None, 5, 1).alias("arcsen", "arcsin"));
}
