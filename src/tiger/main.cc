#include "parser.h"
#include "../utils/file_reader.h"
#include "../utils/printer.h"

#include <iostream>
#include <vector>


void DoParse(const std::string &file) {
    auto reader = FileReader(file.c_str());
    auto code = reader.ReadAll();
    auto lexer = Lexer(code);
    auto tokens = lexer.GetAllTokens();
    auto parser = Parser(std::move(tokens));
    auto ast = parser.ParseResult();
    printf("\n%s\n", ast->ToString(0).c_str());
}

int main() {
    auto prefix = std::string("/Users/xiebei/tiger.cc/test/testcases/");
    auto file = prefix + "test.tig";
    DoParse(file);
}