#pragma once

#include <string>
#include <vector>

#include "Token.hpp"
#include "reader/Reader.hpp"
#include "reader/CodeLoc.hpp"

class Lexer {
public:
    Lexer();

    const Token &getToken() const;
    void nextToken();
    CodeLoc getCodeLoc() const;
    bool isEOF() const;

private:
    Reader reader = Reader("./tests/eval.blu"); // TODO: Change this to take in a file path
    Token currToken = Token(INVALID);
    std::vector<Token> tokens;

    void printTokens() const;
    Token consumeToken();

};