#include <gtest/gtest.h>
#include "tiger/parser.h"
#include "utils/file_reader.h"

void DoParse(const std::string &file) {
    auto reader = FileReader(file.c_str());
    auto code = reader.ReadAll();
    auto lexer = Lexer(code);
    auto tokens = lexer.GetAllTokens();
    auto parser = Parser(std::move(tokens));
    auto ast = parser.ParseResult();
    printf("\n%s\n", ast->ToString(0).c_str());
}

TEST(ALL, ALL_CASES) {
    auto files = std::vector<std::string>();
    for (int i = 1; i < 50; ++i) {
        auto filename = "test" + std::to_string(i) + ".tig";
        files.push_back(filename);
    }
    auto prefix = std::string("/Users/xiebei/tiger.cc/test/testcases/");
    files.emplace_back("queens.tig");
    files.emplace_back("merge.tig");

    for (auto file : files) {
        file = prefix + file;
        printf("%s parse begin~\n", file.c_str());
        DoParse(file);
        printf("%s parse ok!\n", file.c_str());
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}