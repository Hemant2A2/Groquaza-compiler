#include <iostream>
#include <filesystem>
#include "parser/Parser.hpp"
#include "ast/PrintAST.hpp"
#include "lexer/Lexer.hpp"
#include "code-generator/CodeGenerator.hpp"

int main() {
    std::string path = "";

    #ifdef __APPLE__
        path = "/Users/hemantayuj/Desktop/compiler/Groquaza/tests/sum.blu";
    #elif defined(__linux__)
        path = "/home/h3m/Desktop/Projects/Groquaza-compiler/tests/sum.blu";
    #else
        #error "Unsupported device";
    #endif

    std::filesystem::path filePath(path);
    Lexer lex(filePath);
    Parser parser(lex);
    StartNode *startNode = parser.parse();
    printAST(startNode);
    CodeGenerator codeGen("output_sum.s", parser.getSymbolTable());
    codeGen.generateAssembly(startNode);
    return 0;
}