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
    void nextToken();
    CodeLoc getCodeLoc() const;
    bool isEOF() const;
    void expect(TokenType expType);
    void printTokens() const;

private:
    Reader reader;
    Token currToken = Token(INVALID);
    std::vector<Token> tokens;

    Token consumeToken();

};