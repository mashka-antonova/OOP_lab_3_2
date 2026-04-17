#ifndef CALCULATOR_FACADE_H
#define CALCULATOR_FACADE_H

#include <memory>
#include <string>
#include <vector>

#include "calculator_state.h"
#include "command_manager.h"
#include "evaluator.h"
#include "i_command.h"
#include "memory.h"
#include "operator_registry.h"
#include "shunting_yard.h"
#include "token.h"

class CalculatorFacade
{
public:
    CalculatorFacade();

    void addNumber(double value);
    void addBinaryOperator(char op);
    void addUnaryFunction(const std::string& name);
    void addOpenParen();
    void addCloseParen();

    void clearTokenQueue();
    void removeLastToken();
    double calculateQueue();

    void computeAndSave(const std::string& expression, double calculatedResult);

    void memoryAdd(double value);
    void memorySubtract(double value);
    void memoryClear();
    double memoryRecall() const;

    void addToMemory(double value);
    void subtractFromMemory(double value);
    void clearMemory();

    const std::string& getCurrentExpression() const;
    void setCurrentExpression(const std::string& expression);
    double getCurrentResult() const;
    void setCurrentResult(double value);

    void runCommand(std::unique_ptr<ICommand> cmd);
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

    bool isTokenQueueEmpty() const;
    bool lastTokenIsBinaryOperator() const;
    bool lastTokenIsOpenParen() const;

private:
    OperatorRegistry operatorRegistry;
    std::vector<std::unique_ptr<Token>> tokenQueue;
    ShuntingYard shuntingYard;
    Evaluator evaluator;
    Memory memory;
    CommandManager commandManager;
    CalculatorState state;

    bool isUnaryContext() const;
};

#endif // CALCULATOR_FACADE_H
