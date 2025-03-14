#pragma once

#include <stack>

#include "../lexer/Lexer.hpp"
#include "../ast/ASTNodes.hpp"

class Parser {
public:
    Parser(Lexer &lexer) : lexer(lexer) {}

    StartNode *parse();
    StatementNode *parseStatement();
    ExpNode *parseExp();
    AddExpNode *parseAddExp(Token lhs_token);
    BinaryOpNode *parseBinaryOp();
    ComparisonNode *parseComparison();
    KeywordNode *parseKeyword();
    LiteralNode *parseLiteral();
    DataTypeNode *parseDataType();

private:
    Lexer &lexer;
    std::stack<Node *> nodeStack;
    std::vector<std::unique_ptr<Node>> astNodes;

    template<typename T> T *createNode();
    template<typename T> T *endNode(T *node);
};