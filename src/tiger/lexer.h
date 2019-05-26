#include "token.h"

#include <memory>


class Lexer {
public:
    using TokenPtr = std::unique_ptr<Token>;

public:
    Lexer(std::string &&stream): stream_(stream) {}

    TokenPtr NextToken();
    TokenPtr CurrToken();

private:
    char NextChar();

private:
    std::string stream_;
    u64 index_ {0};
    TokenPtr curr_token_;
};