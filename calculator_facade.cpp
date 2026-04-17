#include "calculator_facade.h"

#include "binary_operator.h"
#include "calculate_command.h"
#include "memory_add_command.h"
#include "memory_clear_command.h"
#include "memory_subtract_command.h"
#include "number.h"
#include "unary_operator.h"

CalculatorFacade::CalculatorFacade()
    : operatorRegistry()
    , shuntingYard()
    , evaluator()
    , memory()
    , commandManager()
    , state()
{}

void CalculatorFacade::addNumber(double value) {
    tokenQueue.push_back(std::make_unique<Number>(value));
}

void CalculatorFacade::addBinaryOperator(char op) {
    if ((op == '+' || op == '-') && isUnaryContext()) {
        const std::string unaryName = std::string("unary") + op;
        tokenQueue.push_back(operatorRegistry.createUnaryOperator(unaryName));
    } else {
        tokenQueue.push_back(operatorRegistry.createBinaryOperator(op));
    }
}

void CalculatorFacade::addUnaryFunction(const std::string& name) {
    tokenQueue.push_back(operatorRegistry.createUnaryOperator(name));
}

void CalculatorFacade::addOpenParen() {
    tokenQueue.push_back(operatorRegistry.createUnaryOperator("("));
}

void CalculatorFacade::addCloseParen() {
    tokenQueue.push_back(operatorRegistry.createUnaryOperator(")"));
}

void CalculatorFacade::clearTokenQueue() {
    tokenQueue.clear();
}

void CalculatorFacade::removeLastToken() {
    if (!tokenQueue.empty())
        tokenQueue.pop_back();
}

double CalculatorFacade::calculateQueue() {
    auto rpn = shuntingYard.convertToRPN(std::move(tokenQueue));
    tokenQueue.clear();
    return evaluator.evaluate(rpn);
}

void CalculatorFacade::computeAndSave(const std::string& expression, double calculatedResult) {
    runCommand(std::make_unique<CalculateCommand>(this, expression, calculatedResult));
}

void CalculatorFacade::memoryAdd(double value) {
    memory.add(value);
}

void CalculatorFacade::memorySubtract(double value) {
    memory.subtract(value);
}

void CalculatorFacade::memoryClear() {
    memory.clear();
}

double CalculatorFacade::memoryRecall() const {
    return memory.get();
}

void CalculatorFacade::addToMemory(double value) {
    runCommand(std::make_unique<MemoryAddCommand>(this, value));
}

void CalculatorFacade::subtractFromMemory(double value) {
    runCommand(std::make_unique<MemorySubtractCommand>(this, value));
}

void CalculatorFacade::clearMemory() {
    runCommand(std::make_unique<MemoryClearCommand>(this));
}

const std::string& CalculatorFacade::getCurrentExpression() const {
    return state.getExpression();
}

void CalculatorFacade::setCurrentExpression(const std::string& expression) {
    state.setExpression(expression);
}

double CalculatorFacade::getCurrentResult() const {
    return state.getResult();
}

void CalculatorFacade::setCurrentResult(double value) {
    state.setResult(value);
}

void CalculatorFacade::runCommand(std::unique_ptr<ICommand> cmd) {
    commandManager.execute(std::move(cmd));
}

void CalculatorFacade::undo() {
    commandManager.undo();
}

void CalculatorFacade::redo() {
    commandManager.redo();
}

bool CalculatorFacade::canUndo() const {
    return commandManager.canUndo();
}

bool CalculatorFacade::canRedo() const {
    return commandManager.canRedo();
}

bool CalculatorFacade::isTokenQueueEmpty() const {
    return tokenQueue.empty();
}

bool CalculatorFacade::lastTokenIsBinaryOperator() const {
    return !tokenQueue.empty()
    && dynamic_cast<const BinaryOperator*>(tokenQueue.back().get()) != nullptr;
}

bool CalculatorFacade::lastTokenIsOpenParen() const {
    if (tokenQueue.empty()) return false;
    const auto* unary = dynamic_cast<const UnaryOperator*>(tokenQueue.back().get());
    return unary != nullptr && unary->isOpenParen();
}

bool CalculatorFacade::isUnaryContext() const {
    return isTokenQueueEmpty() || lastTokenIsBinaryOperator() || lastTokenIsOpenParen();
}

