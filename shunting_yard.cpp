#include "shunting_yard.h"

#include "unary_operator.h"
#include <stdexcept>

std::vector<std::unique_ptr<Token>> ShuntingYard::convertToRPN(std::vector<std::unique_ptr<Token>> infix)
{
    resetState();
    for (auto& token : infix) {
        currentToken = std::move(token);
        currentToken->accept(*this);
    }
    flushRemainingOperators();
    return std::move(output);
}

void ShuntingYard::visit(const Number& token) {
    output.push_back(std::move(currentToken));
}

void ShuntingYard::visit(const BinaryOperator& token) {
    popOperatorsByPrecedence(token);
    operators.push(std::move(currentToken));
}

void ShuntingYard::visit(const UnaryOperator& token) {
    token.isCloseParen()
    ? handleCloseParen()
    : operators.push(std::move(currentToken));
}


void ShuntingYard::resetState() {
    output.clear();
    while (!operators.empty())
        operators.pop();
}

void ShuntingYard::flushRemainingOperators() {
    while (!operators.empty()) {
        if (isTopOpenParen())
            throw std::runtime_error("ShuntingYard: mismatched open parenthesis");
        output.push_back(std::move(operators.top()));
        operators.pop();
    }
}

void ShuntingYard::handleCloseParen() {
    popUntilOpenParen();
    popFunctionsAboveOpenParen();
}

void ShuntingYard::popOperatorsByPrecedence(const Token& current) {
    while (shouldPopForBinary(current)) {
        output.push_back(std::move(operators.top()));
        operators.pop();
    }
}
void ShuntingYard::popUntilOpenParen() {
    while (!operators.empty() && !isTopOpenParen()) {
        output.push_back(std::move(operators.top()));
        operators.pop();
    }
    if (operators.empty())
        throw std::runtime_error("ShuntingYard: mismatched close parenthesis");
    operators.pop();
}

void ShuntingYard::popFunctionsAboveOpenParen() {
    while (topIsNonParenUnary()) {
        output.push_back(std::move(operators.top()));
        operators.pop();
    }
}

bool ShuntingYard::shouldPopForBinary(const Token& current) const {
    return !operators.empty() && !isTopOpenParen()
           && operators.top()->getPrecedence() >= current.getPrecedence();
}

bool ShuntingYard::isTopOpenParen() const {
    const auto* unary = (!operators.empty()) ?
                            dynamic_cast<const UnaryOperator*>(operators.top().get())
                                             : nullptr;
    return unary != nullptr && unary->isOpenParen();
}

bool ShuntingYard::topIsNonParenUnary() const {
    const auto* unary = (!operators.empty()) ?
                            dynamic_cast<const UnaryOperator*>(operators.top().get())
                                             : nullptr;
    return unary != nullptr && !unary->isOpenParen() && !unary->isCloseParen();
}
