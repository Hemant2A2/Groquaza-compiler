#pragma once

#include "../ast/ASTNodes.hpp"
#include "AsmEmitter.hpp"
#include <string>

class CodeGenerator {
public:
    CodeGenerator(const std::string &outputFile);
    ~CodeGenerator();

    void generateAssembly(StartNode *root);
private:
    void generateStartNode(StartNode *node);
    void generateStatementNode(StatementNode *node);
    void generateExpNode(ExpNode *node);
    void generateAddExpNode(AddExpNode *node);

    Node* findRHS(Node *expNode);
    int getVariableOffset(const std::string &var);

    AsmEmitter emitter;
};
