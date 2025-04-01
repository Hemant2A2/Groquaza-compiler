```
<start> ::= <statement>* END_OF_FILE

<statement> ::= <keyword> '(' <binaryOp> ')' '{' (<exp>* | <statement>) '}'
                | FOR '(' <exp> <binaryOp> ';' <exp> ')' '{' (<exp>* | <statement>* ) '}'
                | <array_decl>
                | <exp>
                | <statement>

<exp> ::=  <dataType>? IDENTIFIER '=' (<literal> | IDENTIFIER | <array_index> | <addExp>) ';'
           | RETURN (<literal> | IDENTIFIER | <array_index>) ';'
           | <array_index> ASSIGN (<literal> | IDENTIFIER) ';'

<array_decl> ::= VECTOR '<' <dataType> '>' IDENTIFIER  '(' (<literal> | IDENTIFIER) ')' ';'

<array_index> ::= IDENTIFIER '[' (<literal> | IDENTIFIER) ']'

<addExp> ::= (<literal> | IDENTIFIER) (PLUS | MINUS) (<literal> | IDENTIFIER)

<binaryOp> ::= (<literal> | IDENTIFIER) <comparison> (<literal> | IDENTIFIER)

<comparison> ::= NOT_EQUAL | EQUAL | GREATER | GREATER_EQUAL | LESS | LESS_EQUAL

<keyword> ::= IF | ELIF | ELSE | WHILE

<dataType> ::= TYPE_STRING | TYPE_INTEGER | TYPE_FLOAT

<literal> ::= STRING_LIT | INTEGER_LIT | FLOAT_LIT

```