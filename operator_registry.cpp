#include "operator_registry.h"

#include <stdexcept>

OperatorRegistry::OperatorRegistry()
{
    binaryProps['+'] = {1};
    binaryProps['-'] = {1};
    binaryProps['*'] = {2};
    binaryProps['/'] = {2};

    unaryProps["unary+"] = {5};
    unaryProps["unary-"] = {5};
    unaryProps["sin"] = {5};
    unaryProps["cos"] = {5};
    unaryProps["tg"] = {5};
    unaryProps["ctg"] = {5};
    unaryProps["sqrt"] = {5};
    unaryProps["recip"] = {5};
    unaryProps["("] = {0};
    unaryProps[")"] = {0};
}

std::unique_ptr<BinaryOperator> OperatorRegistry::createBinaryOperator(char op) const {
    auto it = binaryProps.find(op);
    if (it == binaryProps.end())
        throw std::invalid_argument(std::string("Unknown binary operator: ") + op);
    return std::make_unique<BinaryOperator>(op, it->second);
}

std::unique_ptr<UnaryOperator> OperatorRegistry::createUnaryOperator(const std::string& name) const {
    auto it = unaryProps.find(name);
    if (it == unaryProps.end())
        throw std::invalid_argument("Unknown unary operator: " + name);
    return std::make_unique<UnaryOperator>(name, it->second);
}
