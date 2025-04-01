#pragma once

#include "../ast/ASTNodes.hpp"
#include "../symbol-table/SymbolTable.hpp"
#include "AsmEmitter.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

class CodeGenerator {
public:
    CodeGenerator(const std::string &outputFile, SymbolTable &symTab);
    ~CodeGenerator();

    void generateAssembly(StartNode *root);

private:
    void generateStartNode(StartNode *node);
    void generateStatementNode(StatementNode *node);
    void generateForLoop(StatementNode *node);
    void generateArrayDeclNode(ArrayDeclNode *node);
    void generateArrayIndexNode(ArrayIndexNode *node, bool isAssignment);
    void generateIfElseChain(const std::vector<StatementNode*> &stmts, size_t &i);
    void generateWhile(StatementNode *node);
    void generateExpNode(ExpNode *node);
    void generateAddExpNode(AddExpNode *node);
    void generateBinaryOpNode(BinaryOpNode *node);
    int getVariableOffset(const std::string &var);
    Node *findRhs(Node *node);
    std::string getUniqueLabel(const std::string &base);
    int labelCounter = 0;
    SymbolTable &symbolTable;

    AsmEmitter emitter;
};
