#pragma once
#include <string>
#include <fstream>

class AsmEmitter {
public:
    AsmEmitter(const std::string &filename);
    ~AsmEmitter();

    // Emit a full line of assembly.
    void emitLine(const std::string &line);
    // Emit an instruction with mnemonic, operands, and an optional comment.
    void emitInstruction(const std::string &mnemonic, const std::string &operands, const std::string &comment = "");
    // Emit a label.
    void emitLabel(const std::string &label);

private:
    std::ofstream out;
};
