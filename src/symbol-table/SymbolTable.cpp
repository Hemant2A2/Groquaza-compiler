#include "SymbolTable.hpp"

SymbolTable::SymbolTable() : nextOffset(0) {}

bool SymbolTable::addSymbol(const std::string &name, const std::string &type) {
    if (table.find(name) != table.end()) {
        return false;
    }
    SymbolInfo info;
    info.type = type;
    info.offset = nextOffset;
    nextOffset += 4;
    table[name] = info;
    return true;
}

bool SymbolTable::lookup(const std::string &name, SymbolInfo &info) const {
    auto it = table.find(name);
    if (it != table.end()) {
        info = it->second;
        return true;
    }
    return false;
}
