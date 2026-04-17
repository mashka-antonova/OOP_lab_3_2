#ifndef I_TOKEN_VISITOR_H
#define I_TOKEN_VISITOR_H

class Number;
class BinaryOperator;
class UnaryOperator;

class ITokenVisitor
{
public:
    virtual ~ITokenVisitor() = default;

    virtual void visit(const Number& token) = 0;
    virtual void visit(const BinaryOperator& token) = 0;
    virtual void visit(const UnaryOperator& token) = 0;
};

#endif // I_TOKEN_VISITOR_H
