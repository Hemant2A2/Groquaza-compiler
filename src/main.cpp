#include <iostream>
#include "parser/Parser.hpp"
#include "ast/PrintAST.hpp"
#include "lexer/Lexer.hpp"

int main() {
    Lexer lex;
    Parser parser(lex);
    StartNode *startNode = parser.parse();
    std::cout << "\n=== AST for eval.blu ===\n";
    printAST(startNode);
    return 0;
}