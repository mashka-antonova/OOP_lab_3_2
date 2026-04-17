#ifndef OPERATOR_REGISTRY_H
#define OPERATOR_REGISTRY_H

#include <map>
#include <memory>
#include <string>

#include "binary_operator.h"
#include "unary_operator.h"

class OperatorRegistry final
{
public:
    OperatorRegistry();

    std::unique_ptr<BinaryOperator> createBinaryOperator(char op) const;
    std::unique_ptr<UnaryOperator> createUnaryOperator(const std::string& name) const;

private:
    std::map<char, int> binaryProps;
    std::map<std::string, int> unaryProps;
};

#endif // OPERATOR_REGISTRY_H
