cmake_minimum_required(VERSION 3.13)
project(tiger_cc)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/tiger)

add_executable(tiger_cc
        src/tiger/ast.h
        src/tiger/common.h
        src/tiger/lexer.cc
        src/tiger/lexer.h
        src/tiger/main.cc
        src/tiger/parser.cc
        src/tiger/parser.h
        src/tiger/token.cc
        src/tiger/token.h src/utils/printer.h)
