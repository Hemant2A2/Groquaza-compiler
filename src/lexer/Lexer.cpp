#include "Lexer.hpp"

#include <iostream>
#include <sstream>

Lexer::Lexer() {
    nextToken();

    // // Uncomment this to test Lexing
    // while (getToken().type != END_OF_FILE) {
    //     nextToken();
    // }
}

const Token &Lexer::getToken() const {
    return currToken;
}

void Lexer::nextToken() {
    while(isspace(reader.getChar()) && !reader.isEOF()) {
        reader.nextChar();
    }
    currToken = consumeToken();
    tokens.push_back(currToken);
}

bool Lexer::isEOF() const {
    return reader.isEOF();
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
        }
        if(currChar == '.') {
            num_stream << currChar;
            reader.nextChar();
            while(isdigit(currChar) && !reader.isEOF()) {
                num_stream << currChar;
                reader.nextChar();
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
                return Token(COMPARE, "==", reader.getCodeLoc());
            }
            return Token(ASSIGN, "=", reader.getCodeLoc());
        case '>':
            if(reader.getChar() == '=' && !reader.isEOF()) {
                reader.nextChar();
                return Token(GREATER_EQUAL, ">=", reader.getCodeLoc());
            }
            return Token(GREATER, ">", reader.getCodeLoc());
        case '<':
            if(reader.getChar() == '=' && !reader.isEOF()) {
                reader.nextChar();
                return Token(LESS_EQUAL, "<=", reader.getCodeLoc());
            }
            return Token(LESS, "<", reader.getCodeLoc());
        default:
            return Token(INVALID, std::string(1, currChar), reader.getCodeLoc());

    }

}