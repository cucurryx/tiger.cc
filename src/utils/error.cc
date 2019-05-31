//
// Created by 解贝 on 2019-05-31.
//

#include "error.h"

void Error(const char *msg, int line, const char *filename) {
    fprintf(stderr, "[%s:%d] %s\n", filename, line, msg);
    exit(1);
}