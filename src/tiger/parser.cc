#include "parser.h"

// mapping operator to it's precedence.
// comparison operators are not associative,
// others are left-associative
std::map<std::string, u64> op_prec_m_ = {
        {"*", 8},
        {"/", 8},
        {"+", 4},
        {"-", 4},
        {">=", 2},
        {"<=", 2},
        {"=", 2},
        {"<>", 2},
        {">", 2},
        {"<", 2},
        {"&", 1},
        {"|", 0},
};

AstNodePtr Parser::ParseResult() {
    return nullptr;
}

Lexer::TokenPtr Parser::NextToken() {
    assert(index_ < tokens_.size());
    return tokens_[index_++];
}

Lexer::TokenPtr Parser::CurrToken() {
    assert(index_ < tokens_.size());
    return tokens_[index_];
}

// program ::= decs | exp
AstNodePtr Parser::ParseMain() {
    auto curr = NextToken();
    if (curr == nullptr) {
        return nullptr;
    }

    auto result = AstNodePtr();
    switch (curr->Type()) {
        case Token::Tag::TYPE:
        case Token::Tag::CLASS:
        case Token::Tag::FUNCTION:
        case Token::Tag::PRIMITIVE:
        case Token::Tag::IMPORT:
        case Token::Tag::VAR:
            result = ParseDecs();
        default:
            result = ParseTopExpr();
    }
    return result;
}

AstNodePtr Parser::ParseDecs() {
    auto decs = DecPtrVec();

    return nullptr;
}

AstNodePtr Parser::ParseTopExpr() {
    return nullptr;
}