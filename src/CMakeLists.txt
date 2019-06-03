project(tiger)
set(CMAKE_CXX_STANDARD 17)

add_executable(tiger_compiler
        tiger/main.cc
        tiger/token.cc
        tiger/lexer.cc
        tiger/ast.cc
        tiger/parser.cc
        utils/file_reader.h
        utils/error.h
        utils/printer.h
        utils/error.cc)