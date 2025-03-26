#include "CodeGenerator.hpp"
#include <sstream>
#include <iostream>

CodeGenerator::CodeGenerator(const std::string &outputFile, SymbolTable &symTab)
    : emitter(outputFile), symbolTable(symTab)
{
    emitter.emitLine(".global _start");
    emitter.emitLabel("_start");
}

CodeGenerator::~CodeGenerator() {}

std::string CodeGenerator::getUniqueLabel(const std::string &base) {
    std::ostringstream oss;
    oss << base << "_" << labelCounter++;
    return oss.str();
}

int CodeGenerator::getVariableOffset(const std::string &var) {
    SymbolInfo info;
    if (symbolTable.lookup(var, info)) {
        return info.offset;
    }
    std::cerr << "Error: Variable " << var << " not declared." << std::endl;
    return -1;
}

void CodeGenerator::generateAssembly(StartNode *root) {
    std::vector<StatementNode*> stmts = root->statements();
    for (size_t i = 0; i < stmts.size(); i++) {
        StatementNode *stmt = stmts[i];
        if (stmt->keyword() != nullptr) {
            switch (stmt->keyword()->keyword) {
                case KeywordNode::IF_KEY:
                case KeywordNode::ELIF_KEY:
                    generateIfElseChain(stmts, i);
                    break;
                case KeywordNode::WHILE_KEY:
                    generateWhile(stmt);
                    break;
                case KeywordNode::ELSE_KEY:
                    generateStatementNode(stmt);
                    break;
                default:
                    generateStatementNode(stmt);
                    break;
            }
        } else {
            generateStatementNode(stmt);
        }
    }
    emitter.emitInstruction("mov", "x8, #93", "Exit syscall");
    emitter.emitInstruction("mov", "x0, #0", "Exit code 0");
    emitter.emitInstruction("svc", "#0", "");
}

void CodeGenerator::generateStartNode(StartNode *node) {
    std::vector<StatementNode*> stmts = node->statements();
    for (auto stmt : stmts) {
        generateStatementNode(stmt);
    }
}

void CodeGenerator::generateStatementNode(StatementNode *node) {
    size_t i = 0;
    if (node->keyword() != nullptr) {
        switch (node->keyword()->keyword) {
            case KeywordNode::IF_KEY:
            case KeywordNode::ELIF_KEY:
                generateIfElseChain({node}, i);
                return;
            case KeywordNode::WHILE_KEY:
                generateWhile(node);
                return;
            default:
                break;
        }
    }
    if (node->exp() != nullptr) {
        generateExpNode(node->exp());
    }
}

void CodeGenerator::generateIfElseChain(const std::vector<StatementNode*> &stmts, size_t &i) {
    StatementNode *ifStmt = stmts[i];
    BinaryOpNode *cond = ifStmt->binaryOp();
    if (!cond) {
        std::cerr << "Error: if condition is not a binary operation" << std::endl;
        return;
    }
    int offsetL = getVariableOffset(cond->left_identifier);
    int offsetR = getVariableOffset(cond->right_identifier);
    emitter.emitInstruction("ldr", "w1, [sp, #" + std::to_string(offsetL) + "]", "Load " + cond->left_identifier);
    emitter.emitInstruction("ldr", "w2, [sp, #" + std::to_string(offsetR) + "]", "Load " + cond->right_identifier);
    emitter.emitInstruction("cmp", "w1, w2", "Compare condition");
    
    ComparisonNode *comp = cond->comparison();
    std::string branch = "";
    switch (comp->comparison) {
        case comp->GREATER_COMP:
            branch = "b.le";
            break;
        case comp->GREATER_EQUAL_COMP:
            branch = "b.lt";
            break;
        case comp->LESS_COMP:
            branch = "b.ge";
            break;
        case comp->LESS_EQUAL_COMP:
            branch = "b.gt";
            break;
        case comp->NOT_EQUAL_COMP:
            branch = "b.eq";
            break;
        case comp->EQUAL_COMP:
            branch = "b.ne";
            break;
        default:
            break;
    }
    std::string elseLabel = getUniqueLabel("else");
    emitter.emitInstruction(branch, elseLabel, "Branch if condition false");

    std::vector<ExpNode*> thenExps = ifStmt->exps();
    for (auto exp : thenExps) {
        generateExpNode(exp);
    }
    std::string endLabel = getUniqueLabel("endif");
    if (i + 1 < stmts.size() && stmts[i+1]->keyword() != nullptr &&
        stmts[i+1]->keyword()->keyword == KeywordNode::ELSE_KEY) {
        i++;
        emitter.emitInstruction("b", endLabel, "Jump to end of if");
        emitter.emitLabel(elseLabel);
        std::vector<ExpNode*> elseExps = stmts[i]->exps();
        for (auto exp : elseExps) {
            generateExpNode(exp);
        }
    }
    else {
        emitter.emitInstruction("b", endLabel, "Jump to end of if");
        emitter.emitLabel(elseLabel);
    }
    emitter.emitLabel(endLabel);
}

void CodeGenerator::generateWhile(StatementNode *node) {
    std::string loopStart = getUniqueLabel("while_start");
    std::string loopEnd = getUniqueLabel("while_end");

    emitter.emitLabel(loopStart);

    BinaryOpNode *cond = node->binaryOp();
    if (!cond) {
        std::cerr << "Error: while condition is not a binary operation" << std::endl;
        return;
    }
    generateBinaryOpNode(cond);
    ComparisonNode *comp = cond->comparison();
    std::string branch = "";
    switch (comp->comparison) {
        case comp->GREATER_COMP:
            branch = "b.le";
            break;
        case comp->GREATER_EQUAL_COMP:
            branch = "b.lt";
            break;
        case comp->LESS_COMP:
            branch = "b.ge";
            break;
        case comp->LESS_EQUAL_COMP:
            branch = "b.gt";
            break;
        case comp->NOT_EQUAL_COMP:
            branch = "b.eq";
            break;
        case comp->EQUAL_COMP:
            branch = "b.ne";
            break;
        default:
            break;
    }
    emitter.emitInstruction(branch, loopEnd, "Exit loop if condition false");

    for (size_t i = 2; i < node->children.size(); i++) {
        Node *child = node->children[i];
        if (auto stmt = dynamic_cast<StatementNode*>(child)) {
            generateStatementNode(stmt);
        } else if (auto exp = dynamic_cast<ExpNode*>(child)) {
            generateExpNode(exp);
        }
    }
    emitter.emitInstruction("b", loopStart, "Repeat loop");
    emitter.emitLabel(loopEnd);
}


void CodeGenerator::generateExpNode(ExpNode *node) {
    if (!node->lhs_identifier.empty()) {
        if (!node->children.empty()) {
            Node *rhs = findRhs(node);
            if(rhs) {
                if (auto lit = dynamic_cast<LiteralNode*>(rhs)) {
                    emitter.emitInstruction("mov", "w0, #" + lit->value, "Load literal " + lit->value);
                } else if (auto addExp = dynamic_cast<AddExpNode*>(rhs)) {
                    generateAddExpNode(addExp);
                }
            }
        }
        int offset = getVariableOffset(node->lhs_identifier);
        emitter.emitInstruction("str", "w0, [sp, #" + std::to_string(offset) + "]", "Store " + node->lhs_identifier);
    }
    else {
        if (!node->children.empty()) {
            if (auto lit = dynamic_cast<LiteralNode*>(node->children[0])) {
                emitter.emitInstruction("mov", "w0, #" + lit->value, "Load literal " + lit->value);
            }
        }
        else if (!node->rhs_identifier.empty()) {
            int offset = getVariableOffset(node->rhs_identifier);
            emitter.emitInstruction("ldr", "w0, [sp, #" + std::to_string(offset) + "]", "Load " + node->rhs_identifier);
        }
        emitter.emitInstruction("bl", "print_int", "Call print_int");
    }
}

Node* CodeGenerator::findRhs(Node *expNode) {
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
    } else if (!node->children.empty()) {
        if (auto lit = dynamic_cast<LiteralNode*>(node->children[0])) {
            emitter.emitInstruction("mov", "w1, #" + lit->value, "Load literal " + lit->value);
        }
    }
    if (!node->right_identifier.empty()) {
        int offset = getVariableOffset(node->right_identifier);
        emitter.emitInstruction("ldr", "w2, [sp, #" + std::to_string(offset) + "]", "Load " + node->right_identifier);
    } else if (!node->children.empty()) {
        if (auto lit = dynamic_cast<LiteralNode*>(node->children.back())) {
            emitter.emitInstruction("mov", "w2, #" + lit->value, "Load literal " + lit->value);
        }
    }
    emitter.emitInstruction("add", "w0, w1, w2", "Compute addition");
}

void CodeGenerator::generateBinaryOpNode(BinaryOpNode *node) {
    if (!node->left_identifier.empty()) {
        int offset = getVariableOffset(node->left_identifier);
        emitter.emitInstruction("ldr", "w1, [sp, #" + std::to_string(offset) + "]", "Load " + node->left_identifier);
    }
    if (!node->right_identifier.empty()) {
        int offset = getVariableOffset(node->right_identifier);
        emitter.emitInstruction("ldr", "w2, [sp, #" + std::to_string(offset) + "]", "Load " + node->right_identifier);
    } else if(!node->children.empty()) {
        if (auto lit = dynamic_cast<LiteralNode*>(node->rhs_literal())) {
            emitter.emitInstruction("mov", "w2, #" + lit->value, "Load literal " + lit->value);
        }
    }
    emitter.emitInstruction("cmp", "w1, w2", "Compare operands");
}
