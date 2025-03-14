#pragma once

#include "ASTNodes.hpp"
#include <string>

void printAST(Node* node, const std::string &prefix = "", bool isLast = true);
