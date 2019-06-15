# Tiger Programming Language Grammer

## Syntactic Specifications

[official Extended BNF](https://www.lrde.epita.fr/~tiger/tiger.html):
```bnf
program ::=
      exp
    | decs

exp ::=
# Literals.
        "nil"
    | integer
    | string
    # Array and record creations.
    | type-id "[" exp "]" "of" exp
    | type-id "{ "[ id "=" exp { "," id "=" exp } ] "} "
    # Object creation.
    | "new" type-id
    # Variables, field, elements of an array.
    | lvalue
    # Function call.
    | id "(" [ exp { "," exp }] ")"
    # Method call.
    | lvalue "." id "(" [ exp { "," exp }] ")"
    # Operations.
    | "-" exp
    | exp op exp
    | "(" exps ")"
    # Assignment.
    | lvalue ":=" exp
    # Control structures.
    | "if" exp "then" exp ["else" exp]
    | "while" exp "do" exp
    | "for" id ":=" exp "to" exp "do" exp
    | "break"
    | "let" decs "in" exps "end"
lvalue ::= id
    | lvalue "." id
    | lvalue "[" exp "]"
exps ::= [ exp { ";" exp } ]

decs ::= { dec }
dec ::=
    # Type declaration.
      "type" id "=" ty

    # Class definition (alternative form).
    | "class" id [ "extends" type-id ] "{ " classfields "} "

    # Variable declaration.
    | vardec

    # Function declaration.
    | "function" id "(" tyfields ")" [ ":" type-id ] "=" exp

    # Primitive declaration.
    | "primitive" id "(" tyfields ")" [ ":" type-id ]

    # Importing a set of declarations.
    | "import" string

vardec ::= "var" id [ ":" type-id ] ":=" exp

classfields ::= { classfield }

# Class fields.
classfield ::=
    # Attribute declaration.
    vardec
    # Method declaration.
    | "method" id "(" tyfields ")" [ ":" type-id ] "=" exp

# Types.
ty ::=
# Type alias.
type-id
     # Record type definition.
     | "{ " tyfields  "} "
     # Array type definition.
     | "array" "of" type-id
     # Class definition (canonical form).
     | "class" [ "extends" type-id ] "{ " classfields "} "

tyfields ::= [ id ":" type-id { "," id ":" type-id } ]

type-id ::= id

op ::= "+" | "-" | "*" | "/" | "=" | "<>" | ">" | "<" | ">=" | "<=" | "&" | "|"
```

But, it's left recursion, like `exp ::= exp op exp`. So, it's necessary to transform it to remove left recursion in order to implement our recursive decline parser.

After removing left recursion, part of `exp` would be:
```bnf

program ::= "let" dec "in" stmt-list "end"

exp ::= exp' { op exp' }
exp' ::= 
        # nil-expr
        "nil"
        # int-expr
        | integer
        # str-expr
        | string
        # record-crt
        | type-id "{" [ id "=" exp { "," id "=" exp } ] "}"
        # new-expr
        | "new" type-id
        # fn-call
        | id "(" [ exp { "," exp }] ")"
        # unary expr
        | "-" expr
        # exprs
        | "(" exps ")"
        # if-expr
        | "if" exp "then" exp ["else" exp]
        # while-expr
        | "while" exp "do" exp
        # for-expr
        | "for" id ":=" exp "to" exp "do" exp
        # break-expr
        | "break"
        # let-expr
        | "let" decs "in" exps "end"
        # maybe array creation
        | type-id lvalue-tail
        # maybe lvalue, method call or assginment
        | id lvalue-tail

lvalue-tail ::= 
        # maybe array creation or lvalue
        "[" exp "]" array-or-lvalue
        # maybe method call, assginment or lvalue
        | lalue lalue-last

arrary-or-lvalue ::= 
        # array creation
        "of" exp 
        # lvalue
        | null

lalue ::= id lvalue'

lvalue' ::= 
        # `.id.id`
        "." id lvalue'
        # `[exp].id.id` balabala
        | "[" exp "]" lvalue'
        # just lvalue
        | null

lvalue-last ::= 
        # lvalue
        null
        # method call
        | "." id "(" [ exp { "," exp }] ")"
        # assginment
        | ":=" exp

exps ::= [ exp { ";" exp } ]

decs ::= { dec }

dec ::=
        # Type declaration.
        "type" id "=" ty

        # Class definition (alternative form).
        | "class" id [ "extends" type-id ] "{ " classfields "} "

        # Variable declaration.
        | vardec

        # Function declaration.
        | "function" id "(" tyfields ")" [ ":" type-id ] "=" exp

        # Primitive declaration.
        | "primitive" id "(" tyfields ")" [ ":" type-id ]

        # Importing a set of declarations.
        | "import" string

vardec ::= "var" id [ ":" type-id ] ":=" exp

classfields ::= { classfield }

# Class fields.
classfield ::=
        # Attribute declaration.
        vardec
        # Method declaration.
        | "method" id "(" tyfields ")" [ ":" type-id ] "=" exp

# Types.
ty ::=
        # Type alias.
        type-id
        # Record type definition.
        | "{ " tyfields  "} "
        # Array type definition.
        | "array" "of" type-id
        # Class definition (canonical form).
        | "class" [ "extends" type-id ] "{ " classfields "} "

tyfields ::= [ id ":" type-id { "," id ":" type-id } ]

type-id ::= id
```
