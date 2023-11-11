#pragma once

#include "Rule.h"
#include "GameState.h"

/// Context for executing a rule
struct ExecuteContext {
    GameState& gameState;
};

class RuleExecuteVisitor : public RuleVisitor {
public:
    RuleExecuteVisitor(ExecuteContext& context)
        : context(context) {}

    void visit(BodyRule& rule) override;
    void visit(ForRule& rule) override;
    void visit(MatchRule& rule) override;
    void visit(DiscardRule& rule) override;
    void visit(MessageRule& rule) override;
    void visit(ParallelForRule& rule) override;
    void visit(InputChoiceRule& rule) override;
    void visit(ExtendRule& rule) override;
    void visit(Rule& rule) override;

private:
    ExecuteContext& context;
};
