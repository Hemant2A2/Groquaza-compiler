#pragma once

#include <string>

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
    COMPARE,            //  ==
    GREATER,            //  >
    GREATER_EQUAL,      //  >=
    LESS,               //  <
    LESS_EQUAL,         //  <=    
    IDENTIFIER,         //  string, int, float, etc
    TYPE_STRING,        //  "string"
    TYPE_INTEGER,       //  5
    TYPE_FLOAT,         //  5.5
    STRING_LIT,         //  string
    INTEGER_LIT,        //  int
    FLOAT_LIT,          //  float
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