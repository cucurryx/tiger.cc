project(tiger)
set(CMAKE_CXX_STANDARD 17)

add_executable(tiger_compiler
        tiger/main.cc
        tiger/token.cc
        tiger/lexer.cc
        tiger/ast.cc
        tiger/parser.cc
        tiger/type.cc
        tiger/visitor.h
        tiger/type_checker.cc
        tiger/codegen.cc
        tiger/scope.cc
        tiger/env.cc
        utils/file_reader.h
        utils/error.h
        utils/printer.h
        utils/error.cc utils/stringfy.h tiger/symbol.cc tiger/symbol.h)