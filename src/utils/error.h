//
// Created by 解贝 on 2019-05-28.
//

#ifndef TIGER_CC_ERROR_H
#define TIGER_CC_ERROR_H

#include <string>

#define PANIC(msg) do {                                        \
    Error(msg, __LINE__, __FILE__);   \
} while(0);

void Error(const char *msg, int line, const char *filename);

#endif //TIGER_CC_ERROR_H
