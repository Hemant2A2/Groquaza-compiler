#pragma once
#include <string>
#include <fstream>

class AsmEmitter {
public:
    AsmEmitter(const std::string &filename);
    ~AsmEmitter();

    void emitLine(const std::string &line);
    void emitInstruction(const std::string &mnemonic, const std::string &operands, const std::string &comment = "");
    void emitLabel(const std::string &label);

private:
    std::ofstream out;
};
