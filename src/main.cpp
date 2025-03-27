#include <iostream>
#include <filesystem>
#include "parser/Parser.hpp"
#include "ast/PrintAST.hpp"
#include "lexer/Lexer.hpp"
#include "code-generator/CodeGenerator.hpp"

int main(int argc, char *argv[]) {
    std::string path = "";
    std::string output = "output_nest.s";

    if (argc > 1) {
        path = argv[1];
        if (argc > 2) {
            output = argv[2];
        }
    } else {
        #ifdef __APPLE__
            path = "/Users/hemantayuj/Desktop/compiler/Groquaza/tests/key.blu";
        #elif defined(__linux__)
            path = "/home/h3m/Desktop/Projects/Groquaza-compiler/tests/nest.blu";
        #else
            #error "Unsupported device";
        #endif
    }

    std::filesystem::path filePath(path);
    Lexer lex(filePath);
    Parser parser(lex);
    StartNode *startNode = parser.parse();
    printAST(startNode);
    CodeGenerator codeGen(output, parser.getSymbolTable());
    codeGen.generateAssembly(startNode);
    return 0;
}