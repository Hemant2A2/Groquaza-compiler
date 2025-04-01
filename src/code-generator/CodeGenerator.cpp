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
                    break;
            }
        } else if(stmt->ofType == StatementNode::FOR) {
            generateForLoop(stmt);
        } else if(stmt->ofType == StatementNode::VECTOR) {
            generateArrayDeclNode(dynamic_cast<ArrayDeclNode*>(stmt->children[0]));
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
    std::string elseLabel = getUniqueLabel("else");
    emitter.emitInstruction(branch, elseLabel, "Branch if condition false");

    for (size_t i = 0; i < ifStmt->children.size(); i++) {
        Node *child = ifStmt->children[i];
        if (auto stmt = dynamic_cast<StatementNode*>(child)) {
            generateStatementNode(stmt);
        } else if (auto exp = dynamic_cast<ExpNode*>(child)) {
            generateExpNode(exp);
        }
    }

    std::string endLabel = getUniqueLabel("endif");
    if (i + 1 < stmts.size() && stmts[i+1]->keyword() != nullptr &&
        stmts[i+1]->keyword()->keyword == KeywordNode::ELSE_KEY) {
        i++;
        emitter.emitInstruction("b", endLabel, "Jump to end of if");
        emitter.emitLabel(elseLabel);

        for (size_t j = 0; j < stmts[i]->children.size(); j++) {
            Node *child = stmts[i]->children[j];
            if (auto stmt = dynamic_cast<StatementNode*>(child)) {
                generateStatementNode(stmt);
            } else if (auto exp = dynamic_cast<ExpNode*>(child)) {
                generateExpNode(exp);
            }
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

    for (size_t i = 0; i < node->children.size(); i++) {
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

void CodeGenerator::generateForLoop(StatementNode *node) {
    StatementNode *forStmt = node->getChild<StatementNode>();
    if (auto initExp = dynamic_cast<ExpNode*>(forStmt->children[0])) {
        generateExpNode(initExp);
    }
    std::string loopStart = getUniqueLabel("for_start");
    std::string loopEnd = getUniqueLabel("for_end");
    emitter.emitLabel(loopStart);
    if (auto condOp = dynamic_cast<BinaryOpNode*>(forStmt->children[1])) {
        generateBinaryOpNode(condOp);
        ComparisonNode *comp = condOp->comparison();
        std::string branchInstr;
        if (comp) {
            switch(comp->comparison) {
                case ComparisonNode::LESS_EQUAL_COMP:
                    branchInstr = "b.gt";
                    break;
                case ComparisonNode::LESS_COMP:
                    branchInstr = "b.ge";
                    break;
                case ComparisonNode::GREATER_EQUAL_COMP:
                    branchInstr = "b.lt";
                    break;
                case ComparisonNode::GREATER_COMP:
                    branchInstr = "b.le";
                    break;
                case ComparisonNode::EQUAL_COMP:
                    branchInstr = "b.ne";
                    break;
                case ComparisonNode::NOT_EQUAL_COMP:
                    branchInstr = "b.eq";
                    break;
                default:
                    branchInstr = "b.ne";
                    break;
            }
        } else {
            branchInstr = "b.ne";
        }
        emitter.emitInstruction(branchInstr, loopEnd, "Exit for loop if condition false");
    }

    for(int i = 3; i < forStmt->children.size(); i++) {
        StatementNode *child = dynamic_cast<StatementNode*>(forStmt->children[i]);
        generateStatementNode(child);
    }

    if (auto updateExp = dynamic_cast<ExpNode*>(forStmt->children[2])) {
        generateExpNode(updateExp);
    }
    
    emitter.emitInstruction("b", loopStart, "Repeat for loop");
    emitter.emitLabel(loopEnd);

}


void CodeGenerator::generateArrayDeclNode(ArrayDeclNode *node) {
    int baseOffset = getVariableOffset(node->identifier);
    for (int i = 0; i < std::stoi(node->size); i++) {
        emitter.emitInstruction("mov", "w0, #0", "Initialize " + node->identifier + "[" + std::to_string(i) + "] to 0");
        emitter.emitInstruction("str", "w0, [sp, #" + std::to_string(baseOffset + i * 4) + "]",
                                  "Store 0 at " + node->identifier + "[" + std::to_string(i) + "]");
    }
}


void CodeGenerator::generateArrayIndexNode(ArrayIndexNode *node, bool isAssignment) {
    int baseOffset = getVariableOffset(node->identifier);
    std::string offsetStr;
    bool indexInReg = false;
    if (!node->index_value.empty()) {
        int idx = std::stoi(node->index_value);
        offsetStr = "#" + std::to_string(idx * 4);
    } else if (!node->index_identifier.empty()) {
        int idxVarOffset = getVariableOffset(node->index_identifier);
        emitter.emitInstruction("ldr", "w1, [sp, #" + std::to_string(idxVarOffset) + "]",
                                  "Load index from " + node->index_identifier);
        emitter.emitInstruction("mov", "w2, #4", "Element size");
        emitter.emitInstruction("mul", "w1, w1, w2", "Compute index offset");
        offsetStr = "x1";
        indexInReg = true;
    } else {
        offsetStr = "#0";
    }
    
    emitter.emitInstruction("add", "x3, sp, #" + std::to_string(baseOffset),
                              "Compute base address for " + node->identifier);
    if (!indexInReg) {
        emitter.emitInstruction("add", "x3, x3, " + offsetStr,
                                  "Compute effective address for " + node->identifier);
    } else {
        emitter.emitInstruction("add", "x3, x3, " + offsetStr,
                                  "Add register offset");
    }
    
    if (!isAssignment) {
        emitter.emitInstruction("ldr", "w0, [x3]", "Load value from " + node->identifier + " element");
    } else {
        emitter.emitInstruction("str", "w0, [x3]", "Store value into " + node->identifier + " element");
    }
}


void CodeGenerator::generateExpNode(ExpNode *node) {
    if (node->lhs_identifier.empty() && !node->children.empty()) {
        if (auto arrIdx = dynamic_cast<ArrayIndexNode*>(node->children[0])) {
            if (!node->rhs_identifier.empty()) {
                int offset = getVariableOffset(node->rhs_identifier);
                emitter.emitInstruction("ldr", "w0, [sp, #" + std::to_string(offset) + "]",
                                          "Load value of " + node->rhs_identifier);
            } else if(node->children.size() > 1) {
                if(auto lit = dynamic_cast<LiteralNode*>(node->children[1])) {
                    emitter.emitInstruction("mov", "w0, #" + lit->value, "Load literal " + lit->value);
                } 
            }
            generateArrayIndexNode(arrIdx, true);
            return;
        }
    }

    if (!node->lhs_identifier.empty()) {
        if (!node->children.empty()) {
            Node *rhs = findRhs(node);
            if(rhs) {
                if (auto lit = dynamic_cast<LiteralNode*>(rhs)) {
                    emitter.emitInstruction("mov", "w0, #" + lit->value, "Load literal " + lit->value);
                } else if (auto addExp = dynamic_cast<AddExpNode*>(rhs)) {
                    generateAddExpNode(addExp);
                } else if(auto arrIdx = dynamic_cast<ArrayIndexNode*>(rhs)) {
                    generateArrayIndexNode(arrIdx, false);
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
        if (dynamic_cast<LiteralNode*>(child) || dynamic_cast<AddExpNode*>(child)
            || dynamic_cast<ArrayIndexNode*>(child))
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
