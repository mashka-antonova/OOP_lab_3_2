#ifndef TOKEN_H
#define TOKEN_H

#include "i_token_visitor.h"

class Token
{
public:
    virtual ~Token() = default;

    virtual void accept(ITokenVisitor& visitor) const = 0;

    virtual int getPrecedence() const {
        return 0;
    }
};

#endif // TOKEN_H
