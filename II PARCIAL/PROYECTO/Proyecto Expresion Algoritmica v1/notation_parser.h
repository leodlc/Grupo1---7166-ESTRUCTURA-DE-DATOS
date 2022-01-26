#pragma once
#include "operator.h"
#include "binary_tree.h"
#include "tree_node.h"
#include "token.h"

#include <string>
#include <vector>
#include <optional>

class ParserResult {
public:
    enum class Status {
        Ok,
        Error
    };

    ParserResult(Status status, std::string message) {
        status_ = status;
        message_ = message;
    }

    Status status() {
        return status_;
    }

    std::string message() {
        return message_;
    }

private:
    Status status_;
    std::string message_;
};

class NotationParser {
public:
    explicit NotationParser(std::string expression);
    BinaryTree<Token> parseTree();
    double calculate();
    

private:
    std::string expression_;
    std::vector<Operator> operators_;

    std::optional<Operator> getOperator(std::string symbol);
    bool hasValidParenthesis();
    void registerDefaultOperators();
    double evaluate(TreeNode<Token>* root);
};
