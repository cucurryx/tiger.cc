# Tiger Programming Language Grammer

## Syntactic Specifications

[official Extended BNF](https://www.lrde.epita.fr/~tiger/tiger.html):
```bnf
program ::=
      exp
    | decs

exp ::=
# Literals.
        ‘nil’
    | integer
    | string
    # Array and record creations.
    | type-id ‘[’ exp ‘]’ ‘of’ exp
    | type-id ‘{ ’[ id ‘=’ exp { ‘,’ id ‘=’ exp } ] ‘} ’
    # Object creation.
    | ‘new’ type-id
    # Variables, field, elements of an array.
    | lvalue
    # Function call.
    | id ‘(’ [ exp { ‘,’ exp }] ‘)’
    # Method call.
    | lvalue ‘.’ id ‘(’ [ exp { ‘,’ exp }] ‘)’
    # Operations.
    | ‘-’ exp
    | exp op exp
    | ‘(’ exps ‘)’
    # Assignment.
    | lvalue ‘:=’ exp
    # Control structures.
    | ‘if’ exp ‘then’ exp [‘else’ exp]
    | ‘while’ exp ‘do’ exp
    | ‘for’ id ‘:=’ exp ‘to’ exp ‘do’ exp
    | ‘break’
    | ‘let’ decs ‘in’ exps ‘end’
lvalue ::= id
    | lvalue ‘.’ id
    | lvalue ‘[’ exp ‘]’
exps ::= [ exp { ‘;’ exp } ]

decs ::= { dec }
dec ::=
    # Type declaration.
      ‘type’ id ‘=’ ty

    # Class definition (alternative form).
    | ‘class’ id [ ‘extends’ type-id ] ‘{ ’ classfields ‘} ’

    # Variable declaration.
    | vardec

    # Function declaration.
    | ‘function’ id ‘(’ tyfields ‘)’ [ ‘:’ type-id ] ‘=’ exp

    # Primitive declaration.
    | ‘primitive’ id ‘(’ tyfields ‘)’ [ ‘:’ type-id ]

    # Importing a set of declarations.
    | ‘import’ string

vardec ::= ‘var’ id [ ‘:’ type-id ] ‘:=’ exp

classfields ::= { classfield }

# Class fields.
classfield ::=

# Attribute declaration.
vardec
    # Method declaration.
    | ‘method’ id ‘(’ tyfields ‘)’ [ ‘:’ type-id ] ‘=’ exp

# Types.
ty ::=
# Type alias.
type-id
     # Record type definition.
     | ‘{ ’ tyfields  ‘} ’
     # Array type definition.
     | ‘array’ ‘of’ type-id
     # Class definition (canonical form).
     | ‘class’ [ ‘extends’ type-id ] ‘{ ’ classfields ‘} ’

tyfields ::= [ id ‘:’ type-id { ‘,’ id ‘:’ type-id } ]

type-id ::= id

op ::= ‘+’ | ‘-’ | ‘*’ | ‘/’ | ‘=’ | ‘<>’ | ‘>’ | ‘<’ | ‘>=’ | ‘<=’ | ‘&’ | ‘|’
```

But, it's left recursion, like `exp ::= exp op exp`. So, it's necessary to transform it to remove left recursion in order to implement our recursive decline parser.

After removing left recursion, part of `exp` would be:
```bnf
exp ::= primary-exp { op exp }

primary-exp ::= 
    'nil'
  | integer
  | string
  | type-id '[' exp ']' 'of' exp
  | type-id '{' [id '=' exp { ',' id '=' exp } ] '}'
  | 'new' type-id
  | lvalue
  | id '(' [ exp { ',' exp }] ')'
  | lvalue '.' id '(' [ exp { ',' exp }] ')'
  | '-' exp
  | '(' exps ')'
  | lvalue ':=' exp
  | 'if' exp 'then' exp ['else' exp]
  | 'while' exp 'do' exp
  | 'for' id ':=' exp 'to' exp 'do' exp
  | 'break'
  | 'let' decs 'in' exps 'end'

lvalue ::= id lvalue'

lvalue' ::= 
    '.' id lvalue'
  | '[' exp ']' lvalue'
  | epsilon

exps ::= [ exp { ';' exp } ]
```

