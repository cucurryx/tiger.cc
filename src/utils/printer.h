#ifndef TIGER_CC_PRINTER_H
#define TIGER_CC_PRINTER_H

#include "../tiger/lexer.h"

template <typename T>
class Printer {
public:
    static std::string print(const T &val) {}
};

template <>
class Printer<Token> {
public:
    static std::string print(const Token &token) {
        return token.Name() + "(" + token.Value() + ")";
    }
};

#endif //TIGER_CC_PRINTER_H
