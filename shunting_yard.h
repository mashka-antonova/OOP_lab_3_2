#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include "i_token_visitor.h"
#include "token.h"

#include <memory>
#include <stack>
#include <vector>

class ShuntingYard : public ITokenVisitor
{
public:
    std::vector<std::unique_ptr<Token>> convertToRPN(std::vector<std::unique_ptr<Token>> infix);

    void visit(const Number& token) override;
    void visit(const BinaryOperator& token) override;
    void visit(const UnaryOperator& token) override;

private:
    std::vector<std::unique_ptr<Token>> output;
    std::stack<std::unique_ptr<Token>> operators;
    std::unique_ptr<Token> currentToken;

    void resetState();
    void flushRemainingOperators();
    void handleCloseParen();

    void popOperatorsByPrecedence(const Token& current);
    void popUntilOpenParen();
    void popFunctionsAboveOpenParen();

    bool shouldPopForBinary(const Token& current) const;
    bool isTopOpenParen() const;
    bool topIsNonParenUnary() const;
};

#endif // SHUNTING_YARD_H
