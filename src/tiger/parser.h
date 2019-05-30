#ifndef TIGER_CC_PARSER_H
#define TIGER_CC_PARSER_H

#include "lexer.h"
#include "ast.h"

#include <map>

class Parser {
public:
    using AstNodePtr = std::unique_ptr<AstNode>;
public:
    Parser(Lexer::TokenPtrVec &&tokens):
        tokens_(std::move(tokens)) {}

    AstNodePtr ParseResult();

private:
    Lexer::TokenPtr NextToken();
    Lexer::TokenPtr CurrToken();

private:
    Lexer::TokenPtrVec tokens_;

    u32 index_ {0};
    static std::map<std::string, u64> op_prec_m_;
};

#endif // TIGER_CC_PARSER_H