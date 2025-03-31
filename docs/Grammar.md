```
<start> ::= <statement>* END_OF_FILE

<statement> ::= <keyword> OPEN_PAREN <binaryOp> CLOSE_PAREN 
                    OPEN_BRACE (<exp>* | <statement>) CLOSE_BRACE
                | FOR OPEN_PAREN <exp> <binaryOp> SEMICOLON <exp> CLOSE_PAREN
                    OPEN_BRACE (<exp>* | <statement>* ) CLOSE_BRACE
                | <array_decl>
                | <exp>
                | <statement>

<exp> ::= <dataType>? IDENTIFIER ASSIGN (<literal> | IDENTIFIER | <array_index> | <addExp>) SEMICOLON
           | RETURN (<literal> | IDENTIFIER | <array_index>) SEMICOLON
           | <array_index> ASSIGN (<literal> | IDENTIFIER) SEMICOLON

<array_decl> ::= VECTOR LT <dataType> GT IDENTIFIER 
                        OPEN_PAREN (<literal> | IDENTIFIER) CLOSE_PAREN SEMICOLON

<array_index> ::= IDENTIFIER OPEN_BRACKET (<literal> | IDENTIFIER) CLOSE_BRACKET 

<addExp> ::= (<literal> | IDENTIFIER) (PLUS | MINUS) (<literal> | IDENTIFIER)

<binaryOp> ::= (<literal> | IDENTIFIER) <comparison> (<literal> | IDENTIFIER)

<comparison> ::= NOT_EQUAL | EQUAL | GREATER | GREATER_EQUAL | LESS | LESS_EQUAL

<keyword> ::= IF | ELIF | ELSE | WHILE

<dataType> ::= TYPE_STRING | TYPE_INTEGER | TYPE_FLOAT

<literal> ::= STRING_LIT | INTEGER_LIT | FLOAT_LIT

```