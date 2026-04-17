#include "evaluator.h"

#include "binary_operator.h"
#include "number.h"
#include "unary_operator.h"

#include <stdexcept>

double Evaluator::evaluate(const std::vector<std::unique_ptr<Token>>& rpn) {
    resetState();
    for (const auto& token : rpn)
        token->accept(*this);
    return takeResult();
}

void Evaluator::visit(const Number& token) {
    values.push(token.getValue());
}

void Evaluator::visit(const BinaryOperator& token) {
    applyOperation(std::string(1, token.getName()), 2);
}

void Evaluator::visit(const UnaryOperator& token) {
    applyOperation(token.getName(), 1);
}

void Evaluator::applyOperation(const std::string& name, int arity) {
    std::vector<double> args = popArgs(arity);
    values.push(operationFactory.create(name)->execute(args));
}

std::vector<double> Evaluator::popArgs(int count) {
    std::vector<double> args(count);
    for (int i = count - 1; i >= 0; --i)
        args[i] = popOne();
    return args;
}

double Evaluator::popOne() {
    if (values.empty())
        throw std::runtime_error("Evaluator: not enough operands on the stack");
    double val = values.top();
    values.pop();
    return val;
}

double Evaluator::takeResult() {
    if (values.size() != 1)
        throw std::runtime_error("Evaluator: invalid RPN, stack must contain exactly one value");
    return values.top();
}

void Evaluator::resetState() {
    while (!values.empty())
        values.pop();
}
