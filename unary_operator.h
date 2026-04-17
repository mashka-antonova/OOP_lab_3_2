#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H

#include "token.h"
#include <string>

class UnaryOperator final: public Token {
public:
    UnaryOperator(const std::string& name, int precedence)
        : name(name), precedence(precedence) {}

    void UnaryOperator::accept(ITokenVisitor& visitor) const {
        visitor.visit(*this);
    }

    const std::string& getName() const {
        return name;
    }

    int getPrecedence() const override {
        return precedence;
    }

    bool isOpenParen() const {
        return name == "(";
    }

    bool isCloseParen() const {
         return name == ")";
    }

private:
    std::string name;
    int precedence;
};

#endif // UNARY_OPERATOR_H
