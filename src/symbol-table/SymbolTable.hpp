#pragma once
#include <string>
#include <unordered_map>

struct SymbolInfo {
    std::string type;
    int offset;
};

class SymbolTable {
public:
    SymbolTable();
    bool addSymbol(const std::string &name, const std::string &type);
    bool lookup(const std::string &name, SymbolInfo &info) const;

private:
    std::unordered_map<std::string, SymbolInfo> table;
    int nextOffset;
};
