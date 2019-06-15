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
//    printf("\n%s\n", ast->ToString(0).c_str());
}

int main() {
    auto files = std::vector<std::string>();
    for (int i = 1; i < 50; ++i) {
        auto filename = "test" + std::to_string(i) + ".tig";
        files.push_back(filename);
    }
    auto prefix = std::string("/Users/xiebei/tiger.cc/test/testcases/");

//    files = std::vector<std::string>{"test42.tig"};

    for (auto file : files) {
        file = prefix + file;
        printf("%s parse begin~\n", file.c_str());
        DoParse(file);
        printf("%s parse ok!\n", file.c_str());
    }
}