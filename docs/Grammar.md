```
<start> ::= <statement>* EOF
<statement> ::= <keyword> OPEN_PAREN <binaryOp> CLOSE_PAREN OPEN_BRACE <exp>* CLOSE_BRACE
                | <exp>
<exp> ::=  <dataType> IDENTIFIER ASSIGN (<literal> | IDENTIFIER | <addSub>) SEMICOLON
           | RETURN (<literal> | IDENTIFIER) SEMICOLON
           | <addSub>
<addSub> ::= IDENTIFIER (PLUS | MINUS) IDENTIFIER
<binaryOp> ::= IDENTIFIER <comparison> IDENTIFIER
<comparison> ::= NOT_EQUAL | COMPARE | GREATER | GREATER_EQUAL | LESS | LESS_EQUAL
<keyword> ::= IF | ELIF | ELSE
<dataType> ::= TYPE_STRING | TYPE_INT | TYPE_FLOAT
<literal> ::= STRING_LIT | INT_LIT | FLOAT_LIT

```