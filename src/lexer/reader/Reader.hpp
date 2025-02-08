#pragma once

#include <fstream>
#include <filesystem>

#include "CodeLoc.hpp"

class Reader {
public:
    Reader(const std::filesystem::path &filePath);
    ~Reader();

    char getChar() const;
    CodeLoc getCodeLoc() const;
    void nextChar();
    bool isEOF() const;
    
private:
    std::ifstream file;
    char currChar = '\0';
    CodeLoc currCodeLoc{1ull, 0ull};
};