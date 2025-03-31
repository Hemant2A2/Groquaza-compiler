#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "Token.hpp"
#include "reader/Reader.hpp"
#include "reader/CodeLoc.hpp"

class Lexer {
public:
    Lexer(std::filesystem::path &filePath);

    const Token &getToken() const;
    const Token &peekToken(size_t ahead = 1) const;
    void advance();
    void backward();
    void nextToken();
    CodeLoc getCodeLoc() const;
    bool isEOF() const;
    void expect(TokenType expType);
    void printTokens() const;

private:
    Reader reader;
    size_t currTokIndex = 0;
    Token currToken = Token(INVALID);
    std::vector<Token> tokens;
    bool inVectorDecl = false;

    Token consumeToken();

};