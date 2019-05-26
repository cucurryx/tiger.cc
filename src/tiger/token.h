#include "common.h"

#include <variant>
#include <string>
#include <unordered_map>


class Token {
public:
    enum class Tag {

        // keywords
        TOKEN_ARRAY,
        TOKEN_IF,
        TOKEN_THEN,
        TOKEN_ELSE,
        TOKEN_WHILE,
        TOKEN_FOR,
        TOKEN_TO,
        TOKEN_DO,
        TOKEN_LET,
        TOKEN_IN,
        TOKEN_END,
        TOKEN_OF,
        TOKEN_BREAK,
        TOKEN_NIL,
        TOKEN_FUNCTION,
        TOKEN_VAR,
        TOKEN_TYPE,
        TOKEN_IMPORT,
        TOKEN_PRIMITIVE,

        // oop keywords
        TOKEN_CLASS,
        TOKEN_EXTENDS,
        TOKEN_METHOD,
        TOKEN_NEW,

        // symbols
        TOKEN_COMMA,     // ,
        TOKEN_COLON,     // :
        TOKEN_SEMI,      // ;
        TOKEN_LPAREN,    // (
        TOKEN_RPAREN,    // )
        TOKEN_LSQB,      // [
        TOKEN_RSQB,      // ]
        TOKEN_LBRACE,     // { 
        TOKEN_RBRACE,     // }
        TOKEN_DOT,        // .
        TOKEN_PLUS,       // +
        TOKEN_MINUS,      // -
        TOKEN_STAR,       // *
        TOKEN_DIV,        // /
        TOKEN_EQUA,       // =
        TOKEN_NOT_EQAL,   // <>
        TOKEN_LESS,       // <
        TOKEN_GREATER,    // >
        TOKEN_LEQ,        // <=
        TOKEN_GEQ,        // >=
        TOKEN_AND,        // &
        TOKEN_OR,         // |
        TOKEN_ASSIGN,     // :=

        // white space
        TOKEN_SPACE,

        // end of line
        TOKEN_EOL,

        // strings
        TOKEN_STR,

        // comments
        TOKEN_COMMENT,

        // identifiers
        TOKEN_ID,

        // numbers
        TOKEN_NUM,

        // invalid characters
        TOKEN_INVALID,
    };

public:
    Token(Tag tag, const std::string &var): 
        tag_(tag), var_(var) { }

public:
    Tag tag_;
    std::string var_;

private:
    static const unordered_map<Tag, std::string> tag_name_m_;
};