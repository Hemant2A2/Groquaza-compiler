#include <iostream>
#include <filesystem>
#include "parser/Parser.hpp"
#include "ast/PrintAST.hpp"
#include "lexer/Lexer.hpp"
#include "code-generator/CodeGenerator.hpp"

int main() {
    std::filesystem::path filePath("/Users/hemantayuj/Desktop/compiler/Groquaza/tests/eval.blu");
    Lexer lex(filePath);
    Parser parser(lex);
    StartNode *startNode = parser.parse();
    printAST(startNode);
    CodeGenerator codeGen("output_eval.s");
    codeGen.generateAssembly(startNode);
    return 0;
}