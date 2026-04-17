#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H

#include "token.h"
#include "i_token_visitor.h"

class BinaryOperator final : public Token
{
public:
    BinaryOperator(char name, int precedence)
        : name(name), precedence(precedence) {}

    void accept(ITokenVisitor& visitor) const override {
        visitor.visit(*this);
    }

    char getName() const {
        return name;
    }

    int getPrecedence() const override {
        return precedence;
    }

private:
    char name;
    int precedence;
};

#endif // BINARY_OPERATOR_H
