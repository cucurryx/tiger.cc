#include <gtest/gtest.h>
#include "tiger/lexer.h"

#define ASSERT_TOKEN_EQ(token_ptr, token)                   \
    do {                                                    \
        ASSERT_NE(token_ptr, nullptr);                      \
        ASSERT_EQ(token_ptr->Type(), token.Type());         \
        ASSERT_EQ(token_ptr->Value(), token.Value());       \
    } while (0);

TEST(TestNum, HandleComplexInput) {
    Lexer lexer("1234");
    ASSERT_EQ(lexer.GetNextToken()->Value(), Token(Token::Tag::NUM, "1234").Value());
    ASSERT_EQ(lexer.GetNextToken(), nullptr);
}

TEST(TestComment, HandleSimpleInput) {
    Lexer lexer("/*this is comment*/");
    auto tokens = lexer.GetAllTokens();
    ASSERT_EQ(tokens.size(), 1);
    ASSERT_TOKEN_EQ(tokens.front(), Token(Token::Tag::COMMENT, "/*this is comment*/"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}