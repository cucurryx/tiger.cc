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
    for (auto &token : tokens) {
        std::cout << Printer<Token>::print(*token) << ", ";
    }
    std::cout << std::endl;

    auto parser = Parser(std::move(tokens));
    auto ast = parser.ParseResult();
    printf("\n%s\n", ast->ToString(0).c_str());
}

int main() {
    auto files = std::vector<std::string> {
        "declarations/array.tig",
        "declarations/method1.tig",
        "declarations/method2.tig",
    };
    auto prefix = std::string("/Users/xiebei/tiger.cc/examples/");
    for (auto file : files) {
        file = prefix + file;
        DoParse(file);
    }
}