#pragma once

#include "Evaluator.h"
#include "Expression.h"
#include "GameState.h"

class ExpressionEvaluateVisitor : public ExpressionVisitor {
public:
    ExpressionEvaluateVisitor(const Evaluator& evaluator, const GameState& gameState);
    
    std::unique_ptr<GameEnvironment::Value>
    operator()(const LiteralExpression& expression) override;
    std::unique_ptr<GameEnvironment::Value>
    operator()(const IdentifierExpression& expression) override;
    std::unique_ptr<GameEnvironment::Value>
    operator()(const BinaryExpression& expression) override;
    std::unique_ptr<GameEnvironment::Value>
    operator()(const UnaryExpression& expression) override;

    std::unique_ptr<GameEnvironment::Value>
    operator()([[maybe_unused]] const Expression& expression) override;

private:
    const Evaluator& m_evaluator;
    const GameState& m_gameState;
};
