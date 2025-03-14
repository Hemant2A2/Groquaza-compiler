#include <assert.h>
#include <iostream>

#include "./Parser.hpp"
#include "../lexer/Lexer.hpp"

StartNode *Parser::parse() {
    StartNode *startNode = createNode<StartNode>();
    while (lexer.getToken().type != END_OF_FILE) {
        parseStatement();
    }
    return endNode(startNode);
}

StatementNode *Parser::parseStatement() {
    StatementNode *statementNode = createNode<StatementNode>();
    if(parseKeyword() != nullptr) {
        lexer.expect(OPEN_PAREN);
        parseBinaryOp();
        lexer.expect(CLOSE_PAREN);
        lexer.expect(OPEN_BRACE);
        while(lexer.getToken().type != CLOSE_BRACE) {
            parseExp();
        }
        lexer.expect(CLOSE_BRACE);
    } else {
        parseExp();
    }
    
    return endNode(statementNode);
}

ExpNode *Parser::parseExp() {
    ExpNode *expNode = createNode<ExpNode>();

    if(parseDataType() != nullptr) {
        Token lhs_token = lexer.getToken();
        std::cout << "lhs_token: " << lhs_token.lexeme << std::endl;
        lexer.expect(IDENTIFIER);
        expNode->lhs_identifier = lhs_token.lexeme;
        lexer.expect(ASSIGN);
       
        auto lit = parseLiteral();
        if(lit != nullptr) {
            expNode->addChild(lit);
        } else if(lexer.getToken().type == IDENTIFIER) {
            Token rhs_token = lexer.getToken();
            std::cout << "rhs_token: " << rhs_token.lexeme << std::endl;
            lexer.nextToken();
            Token nextToken = lexer.getToken();
            if (nextToken.type == PLUS || nextToken.type == MINUS) {
                // It's an additive expression, so parse it.
                AddExpNode *addExp = parseAddExp();
                expNode->addChild(addExp);
            } else {
                expNode->rhs_identifier = rhs_token.lexeme;
            }
        } else {
            AddExpNode *addExp = parseAddExp();
            expNode->addChild(addExp);
        }
    } else if(lexer.getToken().type == RETURN) {
        lexer.nextToken();
        auto lit = parseLiteral();
        if(lit != nullptr) {
            expNode->addChild(lit);
        } else {
            Token iden_token = lexer.getToken();
            std::cout << "iden_token: " << iden_token.lexeme << std::endl;
            expNode->rhs_identifier = iden_token.lexeme;
            lexer.nextToken();
        }
    }
    lexer.expect(SEMICOLON);
    return endNode(expNode);
}

AddExpNode *Parser::parseAddExp() {
    AddExpNode *addExpNode = createNode<AddExpNode>();
    auto left_lit = parseLiteral();
    if(left_lit != nullptr) {
        addExpNode->addChild(left_lit);
    } else if(lexer.getToken().type == IDENTIFIER) {
        Token lhs_token = lexer.getToken();
        std::cout << "lhs_token:(in add exp) " << lhs_token.lexeme << std::endl;
        addExpNode->left_identifier = lhs_token.lexeme;
        lexer.nextToken();
    }
    Token op_token = lexer.getToken();
    std::cout << "op token(in add exp) " << op_token.lexeme << std::endl;
    if(op_token.type == PLUS || op_token.type == MINUS) {
        addExpNode->addOp = op_token.type == PLUS ? AddExpNode::PLUS_OP : AddExpNode::MINUS_OP;
        lexer.nextToken();
    }
    
    auto right_lit = parseLiteral();
    if(right_lit != nullptr) {
        addExpNode->addChild(right_lit);
    } else if(lexer.getToken().type == IDENTIFIER) {
        Token rhs_token = lexer.getToken();
        std::cout << "rhs_token:(in add exp) " << rhs_token.lexeme << std::endl;
        addExpNode->right_identifier = rhs_token.lexeme;
        lexer.nextToken();
    }
    return endNode(addExpNode);
}

BinaryOpNode *Parser::parseBinaryOp() {
    BinaryOpNode *binaryOpNode = createNode<BinaryOpNode>();
    Token token = lexer.getToken();
    auto left_lit = parseLiteral();
    if(left_lit != nullptr) {
        binaryOpNode->addChild(left_lit);
    } else if(token.type == IDENTIFIER) {
        binaryOpNode->left_identifier = token.lexeme;
        lexer.nextToken();
    }
    ComparisonNode *comp = parseComparison();
    binaryOpNode->addChild(comp);
    token = lexer.getToken();
    auto right_lit = parseLiteral();
    if(right_lit != nullptr) {
        binaryOpNode->addChild(right_lit);
    } else if(token.type == IDENTIFIER) {
        binaryOpNode->right_identifier = token.lexeme;
        lexer.nextToken();
    }
    return endNode(binaryOpNode);
}

ComparisonNode *Parser::parseComparison() {
    Token token = lexer.getToken();
    if( token.type != GREATER && token.type != LESS && 
        token.type != GREATER_EQUAL && token.type != LESS_EQUAL && 
        token.type != EQUAL && token.type != NOT_EQUAL ) {
        return nullptr;
    }
    std::cout << "comp token " << token.lexeme << std::endl;
    ComparisonNode *comparisonNode = createNode<ComparisonNode>();

    switch (token.type) {
        case GREATER:
            comparisonNode->comparison = ComparisonNode::GREATER_COMP;
            break;
        case GREATER_EQUAL:
            comparisonNode->comparison = ComparisonNode::GREATER_EQUAL_COMP;
            break;
        case LESS:
            comparisonNode->comparison = ComparisonNode::LESS_COMP;
            break;
        case LESS_EQUAL:
            comparisonNode->comparison = ComparisonNode::LESS_EQUAL_COMP;
            break;
        case EQUAL:
            comparisonNode->comparison = ComparisonNode::EQUAL_COMP;
            break;
        case NOT_EQUAL:
            comparisonNode->comparison = ComparisonNode::NOT_EQUAL_COMP;
            break;
        default:
            break;
    }
    lexer.nextToken();
    return endNode(comparisonNode);
}


KeywordNode *Parser::parseKeyword() {
    Token token = lexer.getToken();
    if (token.type != IF && token.type != ELSE && token.type != ELIF) {
        return nullptr;
    }

    std::cout << "keyword token: " << token.lexeme << std::endl;
    KeywordNode *keywordNode = createNode<KeywordNode>();

    switch (token.type) {
        case IF:
            keywordNode->keyword = KeywordNode::IF_KEY;
            break;
        case ELSE:
            keywordNode->keyword = KeywordNode::ELSE_KEY;
            break;
        case ELIF:
            keywordNode->keyword = KeywordNode::ELIF_KEY;
            break;
        default:
            break;
    }
    lexer.nextToken();
    return endNode(keywordNode);
}


DataTypeNode *Parser::parseDataType() {
    Token token = lexer.getToken();
    if (token.type != TYPE_INTEGER && 
        token.type != TYPE_FLOAT && 
        token.type != TYPE_STRING) {
        return nullptr;
    }

    DataTypeNode *dataTypeNode = createNode<DataTypeNode>();

    switch (token.type) {
        case TYPE_INTEGER:
            dataTypeNode->dataType = DataTypeNode::INT_TYPE;
            break;
        case TYPE_FLOAT:
            dataTypeNode->dataType = DataTypeNode::FLOAT_TYPE;
            break;
        case TYPE_STRING:
            dataTypeNode->dataType = DataTypeNode::STRING_TYPE;
            break;
        default:
            break;
    }
    std::cout << "data type token: " << token.lexeme << std::endl;
    lexer.nextToken();
    return endNode(dataTypeNode);
}


LiteralNode *Parser::parseLiteral() {
    Token token = lexer.getToken();
    if (token.type != INTEGER_LIT && token.type != FLOAT_LIT && token.type != STRING_LIT) {
         return nullptr;
    }
    std::cout << "literal token: " << token.lexeme << std::endl;
    LiteralNode *literalNode = createNode<LiteralNode>();
    literalNode->literal = (token.type == INTEGER_LIT) ? LiteralNode::INT_LITERAL :
                             (token.type == FLOAT_LIT) ? LiteralNode::FLOAT_LITERAL : LiteralNode::STRING_LITERAL;
    literalNode->value = token.lexeme;
    lexer.nextToken();
    return endNode(literalNode);
}


template<typename T>
T *Parser::createNode() {
    Node *par = nullptr;
    if(!std::is_same_v<T, StartNode>) {
        par = nodeStack.top();
    }
    astNodes.push_back(std::make_unique<T>(par, lexer.getCodeLoc()));
    T *node = static_cast<T *>(astNodes.back().get());

    if(!std::is_same_v<T, StartNode>) {
        nodeStack.top()->addChild(node);
    }
    nodeStack.push(node);
    return node;
}

template<typename T>
T *Parser::endNode(T *node) {
    assert(node == nodeStack.top());
    nodeStack.pop();
    return node;
}