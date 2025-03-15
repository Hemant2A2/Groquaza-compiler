#include "CodeGenerator.hpp"
#include <sstream>
#include <iostream>

int CodeGenerator::getVariableOffset(const std::string &var) {
    if (var == "a") return 0;
    else if (var == "b") return 4;
    else if (var == "c") return 8;
    return 12;
}

CodeGenerator::CodeGenerator(const std::string &outputFile)
    : emitter(outputFile)
{
    emitter.emitLine(".global _start");
    emitter.emitLabel("_start");
}

CodeGenerator::~CodeGenerator() {}

void CodeGenerator::generateAssembly(StartNode *root) {
    generateStartNode(root);
    emitter.emitInstruction("mov", "x8, #93", "Exit syscall");
    emitter.emitInstruction("mov", "x0, #0", "Exit code 0");
    emitter.emitInstruction("svc", "#0", "");
}

void CodeGenerator::generateStartNode(StartNode *node) {
    for (auto stmt : node->statements()) {
        generateStatementNode(stmt);
    }
}

void CodeGenerator::generateStatementNode(StatementNode *node) {
    if (node->exp() != nullptr) {
        generateExpNode(node->exp());
    }
}

void CodeGenerator::generateExpNode(ExpNode *node) {
    if (!node->lhs_identifier.empty()) {
        Node *rhs = findRHS(node);
        if (rhs) {
            if (auto lit = dynamic_cast<LiteralNode*>(rhs)) {
                emitter.emitInstruction("mov", "w0, #" + lit->value, "Load literal " + lit->value);
            } else if (auto addExp = dynamic_cast<AddExpNode*>(rhs)) {
                generateAddExpNode(addExp);
            }
        }
        int offset = getVariableOffset(node->lhs_identifier);
        emitter.emitInstruction("str", "w0, [sp, #" + std::to_string(offset) + "]", "Store in " + node->lhs_identifier);
    }
    else if (!node->rhs_identifier.empty()) {
        int offset = getVariableOffset(node->rhs_identifier);
        emitter.emitInstruction("ldr", "w0, [sp, #" + std::to_string(offset) + "]", "Load " + node->rhs_identifier);
        emitter.emitInstruction("bl", "print_int", "Call print_int");
    }
}

Node* CodeGenerator::findRHS(Node *expNode) {
    for (auto child : expNode->children) {
        if (dynamic_cast<LiteralNode*>(child) || dynamic_cast<AddExpNode*>(child))
            return child;
    }
    return nullptr;
}

void CodeGenerator::generateAddExpNode(AddExpNode *node) {
    if (!node->left_identifier.empty()) {
        int offset = getVariableOffset(node->left_identifier);
        emitter.emitInstruction("ldr", "w1, [sp, #" + std::to_string(offset) + "]", "Load " + node->left_identifier);
    }
    else if (!node->children.empty()) {
        if (auto lit = dynamic_cast<LiteralNode*>(node->children[0])) {
            emitter.emitInstruction("mov", "w1, #" + lit->value, "Load literal " + lit->value);
        }
    }
    if (!node->right_identifier.empty()) {
        int offset = getVariableOffset(node->right_identifier);
        emitter.emitInstruction("ldr", "w2, [sp, #" + std::to_string(offset) + "]", "Load " + node->right_identifier);
    }
    else if (node->children.size() >= 2) {
        if (auto lit = dynamic_cast<LiteralNode*>(node->children[1])) {
            emitter.emitInstruction("mov", "w2, #" + lit->value, "Load literal " + lit->value);
        }
    }
    emitter.emitInstruction("add", "w0, w1, w2", "Compute addition");
}
