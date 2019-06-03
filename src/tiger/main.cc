#include "parser.h"
#include "../utils/file_reader.h"
#include "../utils/printer.h"

#include <iostream>

int main() {
    auto reader = FileReader("/Users/xiebei/tiger.cc/examples/example.tig");
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