project(tiger)

add_executable(tiger
        tiger/main.cc
        tiger/token.cc
        tiger/lexer.cc
        tiger/ast.cc
        tiger/parser.cc)