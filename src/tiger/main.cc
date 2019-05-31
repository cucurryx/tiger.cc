#include "parser.h"
#include "../utils/file_reader.h"
#include "../utils/printer.h"

#include <iostream>

int main() {
    auto reader = FileReader("/Users/xiebei/tiger.cc/examples/example.tig");
    auto code = reader.ReadAll();
    auto lexer = Lexer(code);
    for (auto &token : lexer.GetAllTokens()) {
        std::cout << Printer<Token>::print(*token) << std::endl;
    }
}