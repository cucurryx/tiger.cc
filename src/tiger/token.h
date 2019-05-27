#ifndef TIGER_CC_TOKEN_H
#define TIGER_CC_TOKEN_H

#include "common.h"

#include <variant>
#include <string>
#include <unordered_map>

template <typename K, typename V>
using Map = std::unordered_map<K, V>;

class Lexer;

class Token {
public:
    friend class Lexer;
    
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
        EQUA,       // =
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
    Token(Tag tag, const std::string &var):
        tag_(tag), var_(var) {}

    Token(Tag tag, std::string &&var):
        tag_(tag), var_(var) {}

    const std::string Name() const {
        auto node = tag_name_m_.find(tag_);
        assert(node != tag_name_m_.end());
        return node->second;
    }

    const Tag Type() const {
        return tag_;
    }

    const std::string Value() const {
        return var_;
    }

public:
    Tag tag_;
    std::string var_;

private:
    static const Map<Tag, std::string> tag_name_m_;
};

#endif // TIGER_CC_TOKEN_H