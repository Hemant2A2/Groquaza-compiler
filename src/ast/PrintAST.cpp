#include "PrintAST.hpp"
#include <iostream>

static void printIndent(const std::string &prefix, bool isLast) {
    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");
}

static std::string nodeToString(Node* node) {
    if (auto start = dynamic_cast<StartNode*>(node))
        return "StartNode";
    else if (auto stmt = dynamic_cast<StatementNode*>(node))
        return "StatementNode";
    else if (auto exp = dynamic_cast<ExpNode*>(node)) {
        std::string result = "ExpNode";
        if (!exp->lhs_identifier.empty())
            result += " [LHS: " + exp->lhs_identifier + "]";
        if (!exp->rhs_identifier.empty())
            result += " [RHS: " + exp->rhs_identifier + "]";
        return result;
    }
    else if (auto addExp = dynamic_cast<AddExpNode*>(node)) {
        std::string result = "AddExpNode";
        if (!addExp->left_identifier.empty())
            result += " [LHS: " + addExp->left_identifier + "]";
        if (!addExp->right_identifier.empty())
            result += " [RHS: " + addExp->right_identifier + "]";
        switch (addExp->addOp) {
            case AddExpNode::PLUS_OP:  result += " (Operator: +)"; break;
            case AddExpNode::MINUS_OP: result += " (Operator: -)"; break;
            default: break;
        }
        return result;
    }
    else if (auto binOp = dynamic_cast<BinaryOpNode*>(node)) {
        std::string result = "BinaryOpNode";
        if (!binOp->left_identifier.empty())
            result += " [LHS: " + binOp->left_identifier + "]";
        if (!binOp->right_identifier.empty())
            result += " [RHS: " + binOp->right_identifier + "]";
        return result;
    }
    else if (auto comp = dynamic_cast<ComparisonNode*>(node)) {
        std::string result = "ComparisonNode";
        switch (comp->comparison) {
            case ComparisonNode::GREATER_COMP:         result += " (>)"; break;
            case ComparisonNode::GREATER_EQUAL_COMP:   result += " (>=)"; break;
            case ComparisonNode::LESS_COMP:              result += " (<)"; break;
            case ComparisonNode::LESS_EQUAL_COMP:        result += " (<=)"; break;
            case ComparisonNode::EQUAL_COMP:             result += " (==)"; break;
            case ComparisonNode::NOT_EQUAL_COMP:         result += " (!=)"; break;
            default: break;
        }
        return result;
    }
    else if (auto key = dynamic_cast<KeywordNode*>(node)) {
        std::string result = "KeywordNode";
        switch (key->keyword) {
            case KeywordNode::IF_KEY:    result += " (if)"; break;
            case KeywordNode::ELIF_KEY:  result += " (elif)"; break;
            case KeywordNode::ELSE_KEY:  result += " (else)"; break;
            case KeywordNode::WHILE_KEY: result += " (while)"; break;
            default: break;
        }
        return result;
    }
    else if (auto dt = dynamic_cast<DataTypeNode*>(node)) {
        std::string result = "DataTypeNode";
        switch (dt->dataType) {
            case DataTypeNode::INT_TYPE:    result += " (int)"; break;
            case DataTypeNode::FLOAT_TYPE:  result += " (float)"; break;
            case DataTypeNode::STRING_TYPE: result += " (string)"; break;
            default: break;
        }
        return result;
    }
    else if (auto lit = dynamic_cast<LiteralNode*>(node)) {
        std::string result = "LiteralNode";
        switch (lit->literal) {
            case LiteralNode::INT_LITERAL:    result += " (int)"; break;
            case LiteralNode::FLOAT_LITERAL:  result += " (float)"; break;
            case LiteralNode::STRING_LITERAL: result += " (string)"; break;
            default: break;
        }
        result += " [Value: " + lit->value + "]";
        return result;
    }
    return "Unknown Node";
}

void printAST(Node* node, const std::string &prefix, bool isLast) {
    if (!node) return;
    printIndent(prefix, isLast);
    std::cout << nodeToString(node) << std::endl;

    std::string newPrefix = prefix + (isLast ? "    " : "│   ");
    for (size_t i = 0; i < node->children.size(); i++) {
        bool childIsLast = (i == node->children.size() - 1);
        printAST(node->children[i], newPrefix, childIsLast);
    }
}
