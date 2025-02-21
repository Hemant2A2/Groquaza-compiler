#include <assert.h>

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
    if(parseKeyword()) { // should return NULL if not a keyword
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
    const Token &token = lexer.getToken();
    if(parseDataType()) {
        lexer.expect(IDENTIFIER);
        lexer.expect(ASSIGN);
        if(parseLiteral()) {
            // store
        } else if(token.type == IDENTIFIER) {
            // store the value
        } else { // addExp
            parseAddExp();
        }
        lexer.expect(SEMICOLON);
    } else if(token.type == RETURN) {
        lexer.nextToken();
        if(parseLiteral()) {
            // TODO: store the value somewhere
        } else {
            lexer.expect(IDENTIFIER);
        }
        lexer.expect(SEMICOLON);
    }

}

AddExpNode *Parser::parseAddExp() {
    AddExpNode *addExpNode = createNode<AddExpNode>();
    Token token = lexer.getToken();
    if(parseLiteral()) {
        // store
    } else if(token.type == IDENTIFIER) {
        // store the value
        lexer.nextToken();
    }
    token = lexer.getToken();
    if(token.type == PLUS || token.type == MINUS) {
        lexer.nextToken();
    }
    if(parseLiteral()) {
        // store
    } else if(token.type == IDENTIFIER) {
        // store the value
        lexer.nextToken();
    }
    return endNode(addExpNode);
}

BinaryOpNode *Parser::parseBinaryOp() {
    BinaryOpNode *binaryOpNode = createNode<BinaryOpNode>();
    Token token = lexer.getToken();
    if(parseLiteral()) {
        // store
    } else if(token.type == IDENTIFIER) {
        // store the value
        lexer.nextToken();
    }
    parseComparison();
    token = lexer.getToken();
    if(parseLiteral()) {
        // store
    } else if(token.type == IDENTIFIER) {
        // store the value
        lexer.nextToken();
    }
    return endNode(binaryOpNode);
}

ComparisonNode *Parser::parseComparison() {
    ComparisonNode *comparisonNode = createNode<ComparisonNode>();
    Token token = lexer.getToken();
    if( token.type == GREATER || token.type == LESS || 
        token.type == GREATER_EQUAL || token.type == LESS_EQUAL || 
        token.type == EQUAL || token.type == NOT_EQUAL ) {
        lexer.nextToken();
    } else {
        return nullptr;
    }
    return endNode(comparisonNode);
}

KeywordNode *Parser::parseKeyword() {
    KeywordNode *keywordNode = createNode<KeywordNode>();
    Token token = lexer.getToken();
    if(token.type == IF or token.type == ELSE or token.type == ELIF) {
        lexer.nextToken();
    } else {
        return nullptr;
    }
    return endNode(keywordNode);
}

DataTypeNode *Parser::parseDataType() {
    DataTypeNode *dataTypeNode = createNode<DataTypeNode>();
    Token token = lexer.getToken();
    if(token.type == TYPE_INTEGER or token.type == TYPE_FLOAT or token.type == TYPE_STRING) {
        lexer.nextToken();
    } else {
        return nullptr;
    }
    return endNode(dataTypeNode);
}

LiteralNode *Parser::parseLiteral() {
    LiteralNode *literalNode = createNode<LiteralNode>();
    Token token = lexer.getToken();
    if(token.type == INTEGER_LIT or token.type == FLOAT_LIT or token.type == STRING_LIT) {
        lexer.nextToken();
    } else {
        return nullptr;
    }
    return endNode(literalNode);
}


template<typename T>
T *Parser::createNode() {
    T *node = new T(nodeStack.top(), lexer.peek().codeloc);
    nodeStack.push(node);
    return node;
}

template<typename T>
T *Parser::endNode(T *node) {
    assert(node == nodeStack.top());
    nodeStack.pop();
    return node;
}