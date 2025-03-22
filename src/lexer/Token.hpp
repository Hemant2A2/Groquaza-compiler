#pragma once

#include <string>

#include "reader/CodeLoc.hpp"

enum TokenType {
    OPEN_PAREN,         //  (
    CLOSE_PAREN,        //  )
    OPEN_BRACE,         //  {
    CLOSE_BRACE,        //  }
    COMMA,              //  ,
    DOT,                //  .
    SEMICOLON,          //  ;
    MINUS,              //  -
    PLUS,               //  +
    DIV,                //  /    
    MUL,                //  *
    MOD,                //  %
    NOT,                //  !
    NOT_EQUAL,          //  !=
    ASSIGN,             //  =    
    EQUAL,              //  ==
    GREATER,            //  >
    GREATER_EQUAL,      //  >=
    LESS,               //  <
    LESS_EQUAL,         //  <=    
    IDENTIFIER,         //  variable
    STRING_LIT,         //  "string"
    INTEGER_LIT,        //  5
    FLOAT_LIT,          //  5.5
    TYPE_STRING,        //  string
    TYPE_INTEGER,       //  int
    TYPE_FLOAT,         //  float
    AND,                //  and
    ELSE,               //  else
    ELIF,               //  elif
    FALSE,              //  false
    FOR,                //  for
    IF,                 //  if
    NIL,                //  nil
    OR,                 //  or
    PRINT,              //  print
    RETURN,             //  return
    TRUE,               //  true    
    WHILE,              //  while
    END_OF_FILE,        //  EOF
    INVALID
};

struct Token {
    TokenType type;
    std::string lexeme;
    CodeLoc codeLoc;

    Token(TokenType type) : type(type) {}
    Token(TokenType type, const std::string &lexeme, const CodeLoc &codeLoc) : type(type), lexeme(lexeme), codeLoc(codeLoc) {}
};