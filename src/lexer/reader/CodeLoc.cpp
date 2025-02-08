#include "CodeLoc.hpp"

std::string CodeLoc::print() const {
  return "Ln: " + std::to_string(line) + ", Col: " + std::to_string(column);
}