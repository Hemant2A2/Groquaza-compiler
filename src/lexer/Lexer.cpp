#include "Lexer.hpp"

#include <iostream>
#include <sstream>
#include <assert.h>

Lexer::Lexer(std::filesystem::path &filePath) : reader(filePath) {
    advance();
    while (currToken.type != END_OF_FILE) {
        advance();
    }
}

const Token &Lexer::getToken() const {
    return tokens[currTokIndex];
}

const Token &Lexer::peekToken(size_t ahead) const {
    if(currTokIndex + ahead >= tokens.size()) {
        return tokens.back();
    }
    return tokens[currTokIndex + ahead];
}

void Lexer::advance() {
    while(isspace(reader.getChar()) && !reader.isEOF()) {
        reader.nextChar();
    }
    currToken = consumeToken();
    tokens.push_back(currToken);
}

void Lexer::backward() {
    if(currTokIndex == 0) {
        return;
    }
    currTokIndex--;
}

void Lexer::nextToken() {
    currTokIndex++;
}

bool Lexer::isEOF() const {
    return reader.isEOF();
}

void Lexer::expect(TokenType expType) {
    const Token &curr = getToken();
    if (curr.type != expType) {
        std::cerr << "Lexer::expect() failed: Expected token type " << expType
                  << " but got token type " << curr.type
                  << " with lexeme '" << curr.lexeme << "' at "
                  << "line " << curr.codeLoc.line << ", column " << curr.codeLoc.column << "\n";
    }
    assert(getToken().type == expType);
    nextToken();
}

CodeLoc Lexer::getCodeLoc() const {
    return reader.getCodeLoc();
}

void Lexer::printTokens() const {
    for (const Token &token : tokens) {
        std::cout << "Token: " << token.lexeme << " at line: " << token.codeLoc.line << " column: " << token.codeLoc.column << std::endl;
    }
}

Token Lexer::consumeToken() {
   char currChar = reader.getChar();

   if(reader.isEOF()) {
       return Token(END_OF_FILE, "EOF", reader.getCodeLoc());
   }

    auto consumeKeyword = [&]() -> Token {
        std::stringstream ss;
        ss << currChar;
        reader.nextChar();
        currChar = reader.getChar();
        while(isalnum(currChar) && !reader.isEOF()) {
            ss << currChar;
            reader.nextChar();
            currChar = reader.getChar();
        }
        const std::string &lexeme = ss.str();
        if (lexeme == "and") {
            return Token(AND, lexeme, reader.getCodeLoc());
        } else if(lexeme == "or") {
            return Token(OR, lexeme, reader.getCodeLoc());
        } else if(lexeme == "if") {
            return Token(IF, lexeme, reader.getCodeLoc());
        } else if(lexeme == "else") {
            return Token(ELSE, lexeme, reader.getCodeLoc());
        } else if(lexeme == "elif") {
            return Token(ELIF, lexeme, reader.getCodeLoc());
        } else if(lexeme == "for") {
            return Token(FOR, lexeme, reader.getCodeLoc());
        } else if(lexeme == "while") {
            return Token(WHILE, lexeme, reader.getCodeLoc());
        } else if(lexeme == "print") {
            return Token(PRINT, lexeme, reader.getCodeLoc());
        } else if(lexeme == "return") {
            return Token(RETURN, lexeme, reader.getCodeLoc());
        } else if(lexeme == "true") {
            return Token(TRUE, lexeme, reader.getCodeLoc());
        } else if(lexeme == "false") {
            return Token(FALSE, lexeme, reader.getCodeLoc());
        } else if(lexeme == "nil") {
            return Token(NIL, lexeme, reader.getCodeLoc());
        } else if(lexeme == "string") {
            return Token(TYPE_STRING, lexeme, reader.getCodeLoc());
        } else if(lexeme == "int") {
            return Token(TYPE_INTEGER, lexeme, reader.getCodeLoc());
        } else if(lexeme == "float") {
            return Token(TYPE_FLOAT, lexeme, reader.getCodeLoc());
        } else if(lexeme == "vector") {
            inVectorDecl = true;
            return Token(VECTOR, lexeme, reader.getCodeLoc());
        } else if(lexeme == "out") {
            return Token(OUTPUT, lexeme, reader.getCodeLoc());
        } else if(lexeme == "inp") {
            return Token(INPUT, lexeme, reader.getCodeLoc());
        }
        
        return Token(IDENTIFIER, lexeme, reader.getCodeLoc());
    };

    auto consumeNumber = [&]() -> Token {
        std::stringstream num_stream;
        num_stream << currChar;
        reader.nextChar();
        currChar = reader.getChar();
        while(isdigit(currChar) && !reader.isEOF()) {
            num_stream << currChar;
            reader.nextChar();
            currChar = reader.getChar();
        }
        if(currChar == '.') {
            num_stream << currChar;
            reader.nextChar();
            currChar = reader.getChar();
            while(isdigit(currChar) && !reader.isEOF()) {
                num_stream << currChar;
                reader.nextChar();
                currChar = reader.getChar();
            }
            return Token(FLOAT_LIT, num_stream.str(), reader.getCodeLoc());
        }
        return Token(INTEGER_LIT, num_stream.str(), reader.getCodeLoc());
    };

   if(isalpha(currChar)) {
        return consumeKeyword();
   }
    if(isdigit(currChar)) {
         return consumeNumber();
    }

    reader.nextChar();
    switch(currChar) {
        case '(':
            return Token(OPEN_PAREN, "(", reader.getCodeLoc());
        case ')':
            return Token(CLOSE_PAREN, ")", reader.getCodeLoc());
        case '{':
            return Token(OPEN_BRACE, "{", reader.getCodeLoc());
        case '}':
            return Token(CLOSE_BRACE, "}", reader.getCodeLoc());
        case '[':
            return Token(OPEN_BRACKET, "[", reader.getCodeLoc());
        case ']':
            return Token(CLOSE_BRACKET, "]", reader.getCodeLoc());
        case ',':
            return Token(COMMA, ",", reader.getCodeLoc());
        case '.':
            return Token(DOT, ".", reader.getCodeLoc());
        case ';':
            return Token(SEMICOLON, ";", reader.getCodeLoc());
        case '-':
            return Token(MINUS, "-", reader.getCodeLoc());
        case '+':
            return Token(PLUS, "+", reader.getCodeLoc());
        case '/':
            return Token(DIV, "/", reader.getCodeLoc());
        case '*':
            return Token(MUL, "*", reader.getCodeLoc());
        case '%':
            return Token(MOD, "%", reader.getCodeLoc());
        case '!':
            if(reader.getChar() == '=' && !reader.isEOF()) {
                reader.nextChar();
                return Token(NOT_EQUAL, "!=", reader.getCodeLoc());
            }
            return Token(NOT, "!", reader.getCodeLoc());
        case '=':
            if(reader.getChar() == '=' && !reader.isEOF()) {
                reader.nextChar();
                return Token(EQUAL, "==", reader.getCodeLoc());
            }
            return Token(ASSIGN, "=", reader.getCodeLoc());
        case '>':
            if(reader.getChar() == '=' && !reader.isEOF()) {
                reader.nextChar();
                return Token(GREATER_EQUAL, ">=", reader.getCodeLoc());
            } else if(reader.getChar() == '>' && !reader.isEOF()) {
                reader.nextChar();
                return Token(RIGHT_SHIFT, ">>", reader.getCodeLoc());
            }
            if(inVectorDecl) {
                inVectorDecl = false;
                return Token(GT, ">", reader.getCodeLoc());
            }
            return Token(GREATER, ">", reader.getCodeLoc());
        case '<':
            if(reader.getChar() == '=' && !reader.isEOF()) {
                reader.nextChar();
                return Token(LESS_EQUAL, "<=", reader.getCodeLoc());
            } else if(reader.getChar() == '<' && !reader.isEOF()) {
                reader.nextChar();
                return Token(LEFT_SHIFT, "<<", reader.getCodeLoc());
            }
            if(inVectorDecl) {
                return Token(LT, "<", reader.getCodeLoc());
            }
            return Token(LESS, "<", reader.getCodeLoc());
        default:
            return Token(INVALID, std::string(1, currChar), reader.getCodeLoc());

    }

}