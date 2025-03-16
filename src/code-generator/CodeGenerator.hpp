#pragma once

#include "../ast/ASTNodes.hpp"
#include "AsmEmitter.hpp"
#include <string>
#include <sstream>
#include <vector>

class CodeGenerator {
public:
    CodeGenerator(const std::string &outputFile);
    ~CodeGenerator();

    void generateAssembly(StartNode *root);

private:
    void generateStartNode(StartNode *node);
    void generateStatementNode(StatementNode *node);
    void generateIfElseChain(const std::vector<StatementNode*> &stmts, size_t &i);
    void generateExpNode(ExpNode *node);
    void generateAddExpNode(AddExpNode *node);
    void generateBinaryOpNode(BinaryOpNode *node);
    int getVariableOffset(const std::string &var);
    Node *findRhs(Node *node);
    std::string getUniqueLabel(const std::string &base);
    int labelCounter = 0;

    AsmEmitter emitter;
};
