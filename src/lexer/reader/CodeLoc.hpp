#pragma once

#include <string>

struct CodeLoc {
    uint64_t line;
    uint64_t column;

   std::string print() const;
};