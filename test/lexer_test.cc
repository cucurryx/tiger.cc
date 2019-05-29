#include <gtest/gtest.h>
#include "tiger/lexer.h"

TEST(TestScanNum, HandleComplexInput) {
    Lexer lexer("1234");
    ASSERT_EQ(lexer.GetNextToken()->Value(), Token(Token::Tag::NUM, "1234").Value());
    ASSERT_EQ(lexer.GetNextToken(), nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}