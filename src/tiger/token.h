#ifndef TIGER_CC_TOKEN_H
#define TIGER_CC_TOKEN_H

#include "common.h"
#include "../utils/error.h"

#include <utility>
#include <string>
#include <unordered_map>
#include <unordered_set>

template <typename K, typename V>
using Map = std::unordered_map<K, V>;

template <typename K>
using Set = std::unordered_set<K>;

class Lexer;
class Parser;

class Token {
public:
    friend class Lexer;
    friend class Parser;

public:
    enum class Tag {
        // keywords
        ARRAY,
        IF,
        THEN,
        ELSE,
        WHILE,
        FOR,
        TO,
        DO,
        LET,
        IN,
        END,
        OF,
        BREAK,
        NIL,
        FUNCTION,
        VAR,
        TYPE,
        IMPORT,
        PRIMITIVE,

        // oop keywords
        CLASS,
        EXTENDS,
        METHOD,
        NEW,

        // symbols
        COMMA,     // ,
        COLON,     // :
        SEMI,      // ;
        LPAREN,    // (
        RPAREN,    // )
        LSQUB,     // [
        RSQUB,      // ]
        LBRACE,     // { 
        RBRACE,     // }
        DOT,        // .
        PLUS,       // +
        MINUS,      // -
        STAR,       // *
        DIV,        // /
        EQ,       // =
        NOT_EQAL,   // <>
        LESS,       // <
        GREATER,    // >
        LEQ,        // <=
        GEQ,        // >=
        AND,        // &
        OR,         // |
        ASSIGN,     // :=

        // white space
        SPACE,

        // end of line
        EOL,

        // strings
        STR,

        // comments
        COMMENT,

        // identifiers
        ID,

        // numbers
        NUM,

        // invalid characters
        INVALID,
    };

public:
    Token(Tag tag, std::string &&var):
        tag_(tag), var_(std::move(var)) {}

    Token(Tag tag, const std::string &var):
        tag_(tag), var_(var) {}

    bool operator==(const Token &rhs) {
        return rhs.tag_ == tag_ && rhs.var_ == var_;
    }

    const std::string Name() const {
        auto node = tag_name_m_.find(tag_);
        assert(node != tag_name_m_.end());
        return node->second;
    }

    const Tag Type() const {
        return tag_;
    }

    const std::string Value() const {
        if (tag_ == Tag::EOL) {
            return "\\r\\n";
        }
        return var_;
    }

    const bool IsOperator() const {
        switch (tag_) {
        case Tag::PLUS ... Tag::OR:
            return true;
        default:
            return false;
        }
    }

    static std::string TagStr(Tag tag) {
        auto it = tag_name_m_.find(tag);
        if (it == tag_name_m_.end()) {
            PANIC("invalid token tag")
        }
        return it->second;
    }

    static std::optional<Tag> IsKeyword(const std::string &str) {
        auto it = keywords_m_.find(str);
        if (it == keywords_m_.end()) {
            return std::nullopt;
        } else {
            return it->second;
        }
    }

public:
    Tag tag_;
    std::string var_;

private:
    static const Map<Tag, std::string> tag_name_m_;
    static const Map<std::string, Tag> keywords_m_;
    static const Set<Tag> operator_set;
};

#endif // TIGER_CC_TOKEN_H