#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "../utils/printer.h"

#include <iostream>

int main() {
    auto code = "do while list _main ,+- 121212 0/* hello world *****  */a+ \"\\\"he\n\t\"\"llo\" aaa   ";
    auto lexer = Lexer(code);
    auto token = lexer.GetCurrToken();
    while ((token = lexer.GetNextToken())) {
        std::cout << Printer<Token>::print(*token) << std::endl;
    }
}