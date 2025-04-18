#pragma once

#include <string>
#include <vector>
#include "../lexer/reader/Reader.hpp"
#include <iostream>
#include "../lexer/Token.hpp"

class StatementNode;
class KeywordNode;
class BinaryOpNode;
class ExpNode;
class AddExpNode;
class ComparisonNode;
class DataTypeNode;
class LiteralNode;
class IdentifierNode;
class ArrayDeclNode;
class ArrayIndexNode;
class OutputNode;
class InputNode;

class Node {
public:
    Node(Node *parent, CodeLoc codeloc) : parent(parent), codeloc(std::move(codeloc)) {}
    virtual ~Node() {}
    Node(const Node &) = delete;

    template<typename T>
    void addChild(T *childNode) {
        children.push_back(childNode);
    }

    template<typename T>
    T *getChild(size_t index = 0) {
        return dynamic_cast<T *>(children[index]);
    }

    template<typename T>
    std::vector<T *> getChildren() {
        std::vector<T *> nodes;
        for (Node *child : children) {
            T *childOfType = dynamic_cast<T *>(child);
            if (childOfType) {
                nodes.push_back(childOfType);
            }
        }
        return nodes;
    }

    Node *parent;
    std::vector<Node *> children;
    CodeLoc codeloc;
};

class StartNode: public Node {
public:
    using Node::Node;

    // https://stackoverflow.com/questions/76489630/explanation-of-nodiscard-in-c17
    [[nodiscard]] std::vector<StatementNode *> statements() {
        return getChildren<StatementNode>();
    }
};

class StatementNode: public Node {
public:
    using Node::Node;
    enum stmtType: int {
        NONE,
        FOR,
        VECTOR,
    };
    stmtType ofType = NONE;
    KeywordNode *keyword() {
        return getChild<KeywordNode>();
    }

    BinaryOpNode *binaryOp() {
        return getChild<BinaryOpNode>(1);
    }

    ExpNode *exp() {
        return getChild<ExpNode>();
    }

    std::vector<ExpNode *> exps() {
        return getChildren<ExpNode>();
    }

};

class ExpNode: public Node {
public:
    using Node::Node;
    std::string lhs_identifier;
    std::string rhs_identifier;
    TokenType op_type;

    AddExpNode *addExp() {
        return getChild<AddExpNode>();
    }

    LiteralNode *literal() {
        return getChild<LiteralNode>();
    }

    DataTypeNode *dataType() {
        return getChild<DataTypeNode>();
    }

};

class AddExpNode: public Node {
public:
    using Node::Node;
    LiteralNode *literal() {
        return getChild<LiteralNode>();
    }

    std::string left_identifier;
    std::string right_identifier;
    enum AddOp: int {
        NONE_OP,
        PLUS_OP,
        MINUS_OP,
    };
    AddOp addOp = NONE_OP;
};

class BinaryOpNode: public Node {
public:
    using Node::Node;
    std::string left_identifier;
    std::string right_identifier;

    ComparisonNode *comparison() {
        return getChild<ComparisonNode>();
    }

    LiteralNode *rhs_literal() {
        return getChild<LiteralNode>(1);
    }
};

class ComparisonNode: public Node {
public:
    using Node::Node;
    enum Comparison: int {
        NONE_COMP,
        GREATER_COMP,
        GREATER_EQUAL_COMP,
        LESS_COMP,
        LESS_EQUAL_COMP,
        NOT_EQUAL_COMP,
        EQUAL_COMP,
    };
    Comparison comparison = NONE_COMP;

};

class KeywordNode: public Node {
public:
    using Node::Node;
    enum Keyword: int {
        NONE_KEY,
        IF_KEY,
        ELIF_KEY,
        ELSE_KEY,
        WHILE_KEY,
    };
    Keyword keyword = NONE_KEY;
};

class DataTypeNode: public Node {
public:
    using Node::Node;
    enum DataType: int {
        NONE_TYPE,
        INT_TYPE,
        FLOAT_TYPE,
        STRING_TYPE,
    };
    DataType dataType = NONE_TYPE;
};

class LiteralNode: public Node {
public:
    using Node::Node;
    enum Literal: int {
        NONE_LITERAL,
        INT_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
    };
    Literal literal = NONE_LITERAL;
    std::string value;
};

class IdentifierNode: public Node {
public:
    using Node::Node;
    std::string variable;
};

class ArrayDeclNode: public Node {
public:
    using Node::Node;
    std::string identifier;
    DataTypeNode::DataType elementType = DataTypeNode::NONE_TYPE;
    std::string size;
};


class ArrayIndexNode: public Node {
public:
    using Node::Node;
    std::string identifier;
    std::string literal_value;
    std::string assigned_variable;
    std::string index_value;
    std::string index_identifier;

    std::string getIndex() {
        if(index_value.empty()) {
            return index_identifier;
        }
        return index_value;
    }

    std::string getValue() {
        if(literal_value.empty()) {
            return assigned_variable;
        }
        return literal_value;
    }
};

class OutputNode: public Node {
public:
    using Node::Node;
    std::vector<Node*> output_nodes;
};

class InputNode: public Node {
public:
    using Node::Node;
    std::vector<Node*> input_nodes;
};

