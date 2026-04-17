#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "i_token_visitor.h"
#include "operation_factory.h"
#include "token.h"

#include <memory>
#include <stack>
#include <vector>

class Evaluator : public ITokenVisitor {
public:
    double evaluate(const std::vector<std::unique_ptr<Token>>& rpn);

    void visit(const Number& token) override;
    void visit(const BinaryOperator& token) override;
    void visit(const UnaryOperator& token) override;

private:
    OperationFactory operationFactory;
    std::stack<double> values;

    void applyOperation(const std::string& name, int arity);
    std::vector<double> popArgs(int count);
    double popOne();
    double takeResult();
    void resetState();
};

#endif // EVALUATOR_H
