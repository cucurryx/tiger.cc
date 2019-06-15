cmake_minimum_required(VERSION 3.3)

set(ROOT ../)
set(TIGER ${ROOT}/src/tiger)
set(UTILS ${ROOT}/src/utils)

project(tiger_cc_test)

include(CTest)
enable_testing()

add_subdirectory(googletest)
include_directories(googletest/googletest/include)

include_directories(${ROOT}/src)

add_executable(lexer_test lexer_test.cc ${TIGER}/lexer.cc ${TIGER}/token.cc ${UTILS}/error.cc)
target_link_libraries(lexer_test gtest gtest_main)

add_executable(parser_test
        parser_test.cc
        ${TIGER}/parser.cc
        ${TIGER}/token.cc
        ${TIGER}/lexer.cc
        ${TIGER}/ast.cc
        ${UTILS}/error.cc)

target_link_libraries(parser_test gtest gtest_main)