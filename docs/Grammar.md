```
<start> ::= <statement>* END_OF_FILE
<statement> ::= <keyword> OPEN_PAREN <binaryOp> CLOSE_PAREN OPEN_BRACE <exp>* CLOSE_BRACE
                | <exp>
<exp> ::=  <dataType> IDENTIFIER ASSIGN (<literal> | IDENTIFIER | <addExp>) SEMICOLON
           | RETURN (<literal> | IDENTIFIER) SEMICOLON
           | <addExp>
<addExp> ::= (<literal> | IDENTIFIER) (PLUS | MINUS) (<literal> | IDENTIFIER)
<binaryOp> ::= (<literal> | IDENTIFIER) <comparison> (<literal> | IDENTIFIER)
<comparison> ::= NOT_EQUAL | EQUAL | GREATER | GREATER_EQUAL | LESS | LESS_EQUAL
<keyword> ::= IF | ELIF | ELSE
<dataType> ::= TYPE_STRING | TYPE_INTEGER | TYPE_FLOAT
<literal> ::= STRING_LIT | INTEGER_LIT | FLOAT_LIT

```