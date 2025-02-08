#include "Reader.hpp"

Reader::Reader(const std::filesystem::path &filePath) {
    file.open(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Reader Error: Failed to open file: " + filePath.string());
    }

    nextChar();
}

Reader::~Reader() {
    file.close();
}

char Reader::getChar() const {
    return currChar;
}

CodeLoc Reader::getCodeLoc() const {
    return currCodeLoc;
}

void Reader::nextChar() {
    if (isEOF()) {
        return;
    }

    if (currChar == '\n') {
        currCodeLoc.line++;
        currCodeLoc.column = 0;
    }

    currChar = file.get();
    currCodeLoc.column++;
}

bool Reader::isEOF() const {
    return file.eof();
}