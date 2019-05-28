//
// Created by 解贝 on 2019-05-28.
//

#ifndef TIGER_CC_ERROR_H
#define TIGER_CC_ERROR_H

#include <string>

#define PANIC(msg) do {     \
    Error(msg, __LINE__);   \
} while(0);

void Error(std::string &&msg, int line) {
    fprintf(stderr, "line %d: %s", line, msg.c_str());
    exit(1);
}

#endif //TIGER_CC_ERROR_H
