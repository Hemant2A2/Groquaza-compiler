#include <assert.h>
#include <iostream>

#include "./Parser.hpp"
#include "../lexer/Lexer.hpp"
#include "../lexer/Token.hpp"

StartNode *Parser::parse() {
    StartNode *startNode = createNode<StartNode>();
    while (lexer.getToken().type != END_OF_FILE) {
        parseStatement();
    }
    return endNode(startNode);
}

StatementNode *Parser::parseStatement() {
    StatementNode *statementNode = createNode<StatementNode>();
    Token tokk = lexer.getToken();
    if(tokk.lexeme == "if" or tokk.lexeme == "else" or tokk.lexeme == "elif" 
         or tokk.lexeme == "while") {
        KeywordNode *keyword = parseKeyword();
        if(keyword->keyword != KeywordNode::ELSE_KEY) {
            lexer.expect(OPEN_PAREN);
            parseBinaryOp();
            lexer.expect(CLOSE_PAREN);
        }
        lexer.expect(OPEN_BRACE);
        while(lexer.getToken().type != CLOSE_BRACE) {
            Token tok = lexer.getToken();
            if(tok.lexeme == "if" or tok.lexeme == "else" or tok.lexeme == "elif" 
                or tok.lexeme == "while") {
                parseStatement();
            } else {
                parseExp();
            }
        }
        lexer.expect(CLOSE_BRACE);
    } else if(tokk.lexeme == "for") {
        statementNode->ofType = StatementNode::FOR;
        parseForStatement();
    } else if(tokk.lexeme == "vector") {
        statementNode->ofType = StatementNode::VECTOR;
        parseArrayDecl();
    } else {
        parseExp();
    }
    
    return endNode(statementNode);
}

StatementNode *Parser::parseForStatement() {
    StatementNode *forStmt = createNode<StatementNode>();
    lexer.expect(FOR);
    lexer.expect(OPEN_PAREN);
    parseExp();
    parseBinaryOp();
    lexer.expect(SEMICOLON);
    parseExp();
    lexer.expect(CLOSE_PAREN);
    lexer.expect(OPEN_BRACE);
    while(lexer.getToken().type != CLOSE_BRACE) {
       parseStatement();
    }
    lexer.expect(CLOSE_BRACE);
    return endNode(forStmt);
}

ArrayDeclNode *Parser::parseArrayDecl() {
    ArrayDeclNode *arrayDecl = createNode<ArrayDeclNode>();
    lexer.expect(VECTOR);
    lexer.expect(LT);
    DataTypeNode *elemType = parseDataType();
    lexer.expect(GT);
    Token idToken = lexer.getToken();
    lexer.expect(IDENTIFIER);
    arrayDecl->identifier = idToken.lexeme;
    if (elemType) {
        arrayDecl->elementType = elemType->dataType;
    }
    lexer.expect(OPEN_PAREN);
    Token tok = lexer.getToken();
    auto lit = parseLiteral();
    if(lit != nullptr) {
        arrayDecl->size = lit->value;
    }
    std::string typeStr;
    switch(arrayDecl->elementType) {
        case DataTypeNode::INT_TYPE:    typeStr = "int"; break;
        case DataTypeNode::FLOAT_TYPE:  typeStr = "float"; break;
        case DataTypeNode::STRING_TYPE: typeStr = "string"; break;
        default: typeStr = "unknown"; break;
    }
    if(!symbolTable.addSymbol(arrayDecl->identifier, typeStr, std::stoi(arrayDecl->size))) {
        std::cerr << "Error: Variable " << arrayDecl->identifier << " redeclared." << std::endl;
    }
    lexer.expect(CLOSE_PAREN);
    lexer.expect(SEMICOLON);
    return endNode(arrayDecl);
}

ArrayIndexNode *Parser::parseArrayIndex() {
     ArrayIndexNode *indexNode = createNode<ArrayIndexNode>();
    Token idToken = lexer.getToken();
    lexer.expect(IDENTIFIER);
    indexNode->identifier = idToken.lexeme;
    lexer.expect(OPEN_BRACKET);
    Token idxToken = lexer.getToken();
    if (idxToken.type == INTEGER_LIT) {
        indexNode->index_value = idxToken.lexeme;
    } else if(idxToken.type == IDENTIFIER) {
        indexNode->index_identifier = idxToken.lexeme;
    }
    lexer.nextToken();
    lexer.expect(CLOSE_BRACKET);
    return endNode(indexNode);
}

ExpNode *Parser::parseExp() {
    ExpNode *expNode = createNode<ExpNode>();
    auto dtNode = parseDataType();
    if(dtNode != nullptr) {
        std::string typeStr;
        switch(dtNode->dataType) {
            case DataTypeNode::INT_TYPE:    typeStr = "int"; break;
            case DataTypeNode::FLOAT_TYPE:  typeStr = "float"; break;
            case DataTypeNode::STRING_TYPE: typeStr = "string"; break;
            default: typeStr = "unknown"; break;
        }
        Token lhs_token = lexer.getToken();
        lexer.expect(IDENTIFIER);
        expNode->lhs_identifier = lhs_token.lexeme;
        if(!symbolTable.addSymbol(lhs_token.lexeme, typeStr)) {
            std::cerr << "Error: Variable " << lhs_token.lexeme << " redeclared." << std::endl;
        }
        lexer.expect(ASSIGN);
       
        auto lit = parseLiteral();
        if(lit != nullptr) {
            // expNode->addChild(lit);
        } else if(lexer.getToken().type == IDENTIFIER) {
            Token rhs_token = lexer.getToken();
            Token nextToken = lexer.peekToken();
            if(nextToken.type == PLUS || nextToken.type == MINUS) {
                AddExpNode *addExp = parseAddExp();
            } else if(nextToken.type == SEMICOLON) {
                expNode->rhs_identifier = rhs_token.lexeme;
                lexer.nextToken();
            } else {
                ArrayIndexNode *indexNode = parseArrayIndex();
                expNode->rhs_identifier = indexNode->getValue();
            }
        }
    } else if(lexer.getToken().type == IDENTIFIER) {
        Token varToken = lexer.getToken();
        SymbolInfo info;
        if(!symbolTable.lookup(varToken.lexeme, info)) {
            std::cerr << "Error: Variable " << varToken.lexeme << " not declared." << std::endl;
        }
        Token nextTok = lexer.peekToken();
        if(nextTok.type == OPEN_BRACKET) {
            ArrayIndexNode *indexNode = parseArrayIndex();
            expNode->lhs_identifier = indexNode->identifier + "[" + indexNode->getIndex() + "]";
            lexer.expect(ASSIGN);
            Token value_tok = lexer.getToken();
            auto lit = parseLiteral();
            if(lit != nullptr) {
                indexNode->literal_value = lit->value;
            } else if(value_tok.type == IDENTIFIER) {
                indexNode->assigned_variable = value_tok.lexeme;
                lexer.nextToken();
            }
            expNode->rhs_identifier = indexNode->getValue();
        } else {
            expNode->lhs_identifier = varToken.lexeme;
            lexer.nextToken();
            lexer.expect(ASSIGN);
            auto lit = parseLiteral();
            if(lit != nullptr) {
                // expNode->addChild(lit);
            } else if(lexer.getToken().type == IDENTIFIER) {
                Token rhs_token = lexer.getToken();
                Token nextToken = lexer.peekToken();
                if(nextToken.type == PLUS || nextToken.type == MINUS) {
                    AddExpNode *addExp = parseAddExp();
                } else if(nextToken.type == SEMICOLON) {
                    expNode->rhs_identifier = rhs_token.lexeme;
                } else {
                    ArrayIndexNode *indexNode = parseArrayIndex();
                    expNode->rhs_identifier = indexNode->getValue();
                }
            }
        }
    } else if(lexer.getToken().type == RETURN) {
        lexer.nextToken();
        auto lit = parseLiteral();
        if(lit != nullptr) {
            // expNode->addChild(lit);
        } else {
            Token iden_token = lexer.getToken();
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
        // addExpNode->addChild(right_lit);
    } else if(lexer.getToken().type == IDENTIFIER) {
        Token lhs_token = lexer.getToken();
        addExpNode->left_identifier = lhs_token.lexeme;
        lexer.nextToken();
    }
    
    Token op_token = lexer.getToken();
    if(op_token.type == PLUS || op_token.type == MINUS) {
        addExpNode->addOp = op_token.type == PLUS ? AddExpNode::PLUS_OP : AddExpNode::MINUS_OP;
        lexer.nextToken();
    }
    
    auto right_lit = parseLiteral();
    if(right_lit != nullptr) {
        // addExpNode->addChild(right_lit);
    } else if(lexer.getToken().type == IDENTIFIER) {
        Token rhs_token = lexer.getToken();
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
        // binaryOpNode->addChild(left_lit);
    } else if(token.type == IDENTIFIER) {
        binaryOpNode->left_identifier = token.lexeme;
        lexer.nextToken();
    }
    ComparisonNode *comp = parseComparison();
    // binaryOpNode->addChild(comp);
    token = lexer.getToken();
    auto right_lit = parseLiteral();
    if(right_lit != nullptr) {
        // binaryOpNode->addChild(right_lit);
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
    if (token.type != IF && token.type != ELSE && token.type != ELIF && token.type != WHILE) {
        return nullptr;
    }
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
        case WHILE:
            keywordNode->keyword = KeywordNode::WHILE_KEY;
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
    lexer.nextToken();
    return endNode(dataTypeNode);
}


LiteralNode *Parser::parseLiteral() {
    Token token = lexer.getToken();
    if (token.type != INTEGER_LIT && token.type != FLOAT_LIT && token.type != STRING_LIT) {
         return nullptr;
    }
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

SymbolTable &Parser::getSymbolTable() {
    return symbolTable;
}