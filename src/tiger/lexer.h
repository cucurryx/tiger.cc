#ifndef TIGER_CC_LEXER_H
#define TIGER_CC_LEXER_H

#include "token.h"
#include <memory>

class Lexer {
public:
    using TokenPtr = std::shared_ptr<Token>;

public:
    explicit Lexer(std::string &&stream):
        stream_(stream) {}

    TokenPtr GetNextToken();
    TokenPtr GetCurrToken();

private:
    char Next();
    char Curr();
    bool Is(char c);
    bool Try(char c);
    void SkipSpace();

    TokenPtr ScanId(char c);
    TokenPtr ScanMainId(char c);
    TokenPtr ScanNormalId(char c);

    TokenPtr ScanNum(char c);
    TokenPtr ScanComment();
    TokenPtr ScanString();

    char ParseOctNum();
    char ParseHexNum();
    static u8 HexToDigit(char c);
    static u8 OctToDigit(char c);

    static TokenPtr MakeToken(Token::Tag tag, std::string &&var);
    static TokenPtr MakeToken(Token::Tag tag, const std::string &var);

private:
    std::string stream_;
    u64 index_ {0};
    TokenPtr curr_token_{nullptr};
};

#endif // TIGER_CC_LEXER_H