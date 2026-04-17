#ifndef NUMBER_H
#define NUMBER_H

#include "token.h"
#include "i_token_visitor.h"

class Number : public Token
{
public:

    explicit Number(double num)
        : value(num) {}

    void accept(ITokenVisitor& visitor) const override {
        visitor.visit(*this);
    }

    double getValue() const {
        return value;
    }

private:
    double value;
};

#endif // NUMBER_H
