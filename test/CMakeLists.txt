cmake_minimum_required(VERSION 3.3)

set(ROOT ../)
set(TIGER ${ROOT}/src/tiger)

project(tiger_cc_test)

include(CTest)
enable_testing()

add_subdirectory(googletest)
include_directories(googletest/googletest/include)

include_directories(${ROOT}/src)

add_executable(lexer_test lexer_test.cc ${TIGER}/lexer.cc ${TIGER}/token.cc)
target_link_libraries(lexer_test gtest gtest_main)