#include "lexer.h"
#include "../utils/error.h"

// scan the next token and return it
Lexer::TokenPtr Lexer::GetNextToken() {
    SkipSpace();
    char c = Next();
    switch (c) {
        case ',':
            return MakeToken(Token::Tag::COMMA, ",");
        case ':': {
            if (Try('=')) {
                return MakeToken(Token::Tag::ASSIGN, ":=");
            } else {
                return MakeToken(Token::Tag::COLON, ":");
            }
        }
        case ';':
            return MakeToken(Token::Tag::SEMI, ";");
        case '(':
            return MakeToken(Token::Tag::LPAREN, "(");
        case ')':
            return MakeToken(Token::Tag::RPAREN, ")");
        case '[':
            return MakeToken(Token::Tag::LSQUB, "[");
        case ']':
            return MakeToken(Token::Tag::RSQUB, "]");
        case '{':
            return MakeToken(Token::Tag::LBRACE, "{");
        case '}':
            return MakeToken(Token::Tag::RBRACE, "}");
        case '.':
            return MakeToken(Token::Tag::DOT, ".");
        case '+':
            return MakeToken(Token::Tag::PLUS, "+");
        case '-':
            return MakeToken(Token::Tag::MINUS, "-");
        case '*':
            return MakeToken(Token::Tag::STAR, "*");
        case '/':
            if (Try('*')) {
                return ScanComment();
            } else {
                return MakeToken(Token::Tag::DIV, "/");
            }
        case '=':
            return MakeToken(Token::Tag::EQ, "=");
        case '<':
            if (Try('=')) {
                return MakeToken(Token::Tag::LEQ, "<=");
            } else if (Try('>')) {
                return MakeToken(Token::Tag::NOT_EQAL, "<>");
            } else {
                return MakeToken(Token::Tag::LESS, "<");
            }
        case '>':
            if (Try('=')) {
                return MakeToken(Token::Tag::GEQ, ">=");
            } else {
                return MakeToken(Token::Tag::GREATER, ">");
            }
        case '&':
            return MakeToken(Token::Tag::AND, "&");
        case '|':
            return MakeToken(Token::Tag::OR, "|");
        case '\r':
            if (Try('\n')) {
                return MakeToken(Token::Tag::EOL, "\r\n");
            } else {
                return MakeToken(Token::Tag::EOL, "\r");
            }
        case '\n':
            if (Try('\r')) {
                return MakeToken(Token::Tag::EOL, "\n\r");
            } else {
                return MakeToken(Token::Tag::EOL, "\n");
            }
        case '"':
            return ScanString();
        case 'a' ... 'z': case 'A' ... 'Z': case '_':
            return ScanId(c);
        case '0' ... '9':
            return ScanNum(c);
        case '\0':
            return nullptr;
        default:
            return MakeToken(Token::Tag::INVALID, std::to_string(c));
    }
}

// return current token
Lexer::TokenPtr Lexer::GetCurrToken() {
    return curr_token_;
}

// get all tokens
Lexer::TokenPtrVec Lexer::GetAllTokens() {
    TokenPtrVec tokens;
    while (auto ptr = GetNextToken()) {
        tokens.push_back(ptr);
    }
    return tokens;
}

// return current character and move index to next
char Lexer::Next() {
    if (index_ < stream_.size()) {
        return stream_[index_++];
    } else {
        return '\0';
    }
}

// return current character
char Lexer::Curr() {
    assert(index_ <= stream_.size());
    return stream_[index_];
}

// judge whether current character is c
bool Lexer::Is(char c) {
    return Curr() == c;
}

// if curr character is c, move index to next and return true.
// otherwise, just return false.
bool Lexer::Try(char c) {
    if (Curr() == c) {
        Next();
        return true;
    } else {
        return false;
    }
}

void Lexer::SkipSpace() {
    while (Try(' ') || Try('\t'));
}

Lexer::TokenPtr Lexer::MakeToken(Token::Tag tag, std::string &&var) {
    return std::make_shared<Token>(tag, std::move(var));
}

Lexer::TokenPtr Lexer::MakeToken(Token::Tag tag, const std::string &var) {
    return std::make_shared<Token>(tag, var);
}

char Lexer::ParseOctNum() {
    u8 num = 0;
    for (u8 i = 0; i < 3; ++i) {
        num = (num << 3) + OctToDigit(Next());
    }
    return static_cast<char>(num);
}

char Lexer::ParseHexNum() {
    u8  num = 0;
    for (u8 i = 0; i < 2; ++i) {
        num = (num << 4) + HexToDigit(Next());
    }
    return static_cast<char>(num);
}

u8 Lexer::HexToDigit(char c) {
    if (isdigit(c)) {
        return c - '0';
    } else if (islower(c)) {
        return c - 'a' + 10;
    } else if (isupper(c)) {
        return c - 'A' + 10;
    } else {
        auto msg = std::string(1, c) + " is not hex";
        PANIC(msg.c_str());
    }
    return 0;
}

u8 Lexer::OctToDigit(char c) {
    if (c >= '0' && c <= '7') {
        return c - '0';
    } else {
        auto msg = std::string(1, c) + " is no oct";
        PANIC(msg.c_str());
    }
    return 0;
}

Lexer::TokenPtr Lexer::ScanId(char c) {
    if (c == '_') {
        return ScanMainId(c);
    } else {
        return ScanNormalId(c);
    }
}

Lexer::TokenPtr Lexer::ScanMainId(char c) {
    std::string trace(1, c);
    std::string main = "main";
    for (auto x : main) {
        trace.push_back(x);
        if (!Try(x)) {
            return MakeToken(Token::Tag::INVALID, std::move(trace));
        }
    }
    return MakeToken(Token::Tag::ID, std::move(trace));
}

Lexer::TokenPtr Lexer::ScanNormalId(char c) {
    std::string trace(1, c);
    while (isalnum((c = Curr())) || Is('_')) {
        trace.push_back(c);
        Next();
    }
    if (auto tag = Token::IsKeyword(trace)) {
        return MakeToken(tag.value(), std::move(trace));
    }
    return MakeToken(Token::Tag::ID, std::move(trace));
}

// tiger just support integers
Lexer::TokenPtr Lexer::ScanNum(char c) {
    std::string num(1, c);
    while (isnumber(c = Curr())) {
        num.push_back(c);
        Next();
    }
    return MakeToken(Token::Tag::NUM, std::move(num));
}

Lexer::TokenPtr Lexer::ScanComment() {
    std::string comment = "/*";
    char c;
    for (;;) {
        while ((c = Next()) != '*') {
            comment.push_back(c);
        }
        comment.push_back(c);
        if (Next() == '/') {
            comment += "/";
            return MakeToken(Token::Tag::COMMENT, std::move(comment));
        } else {
            comment.push_back(c);
        }
    }
}

Lexer::TokenPtr Lexer::ScanString() {
    char c;
    std::string s;
    while ((c = Next()) != '\"' && c != '\0') {
        if (c == '\\') {
            c = Next();
            switch (c) {
                case 'a':
                    s.push_back('\a'); break;
                case 'b':
                    s.push_back('\b'); break;
                case 'f':
                    s.push_back('\f'); break;
                case 'n':
                    s.push_back('\n'); break;
                case 'r':
                    s.push_back('\r'); break;
                case 't':
                    s.push_back('\t'); break;
                case 'v':
                    s.push_back('\v'); break;
                case 'x':
                    s.push_back(ParseHexNum()); break;
                case '0' ... '9':
                    s.push_back(ParseOctNum()); break;
                case '\\': case '\"':
                    s.push_back(c); break;
                default:
                    s.push_back(c);
                    return MakeToken(Token::Tag::INVALID, std::move(s));
            }
        } else {
            s.push_back(c);
        }
    }
    if (c == '\0') {
        return MakeToken(Token::Tag::INVALID, "\"" + s);
    } else {
        return MakeToken(Token::Tag::STR, std::move(s));
    }
}
