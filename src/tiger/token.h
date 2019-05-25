#include "common.h"

#include <variant>
#include <string>


class Token {
public:
    enum class Tag {
        TOKEN_DEF,
    };

public:
    Token(Tag tag, const std::string &var): 
        tag_(tag), var_(var) { }

public:
    Tag tag_;
    std::string var_;

private:
    
};