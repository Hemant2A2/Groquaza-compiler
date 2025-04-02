#include "AsmEmitter.hpp"
#include <iostream>

AsmEmitter::AsmEmitter(const std::string &filename) {
    out.open(filename);
    if (!out) {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

AsmEmitter::~AsmEmitter() {
    if (out.is_open()) {
        out.close();
    }
}

void AsmEmitter::emitLine(const std::string &line) {
    out << line << "\n";
}

void AsmEmitter::emitInstruction(const std::string &code, const std::string &operands, const std::string &comment) {
    out << "    " << code;
    if (!operands.empty()) {
        out << " " << operands;
    }
    if (!comment.empty()) {
        out << "        // " << comment;
    }
    out << "\n";
}

void AsmEmitter::emitLabel(const std::string &label) {
    out << label << ":\n";
}
