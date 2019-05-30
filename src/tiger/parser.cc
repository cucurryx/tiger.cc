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

Parser::AstNodePtr Parser::ParseResult() {
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

