//
// Created by 解贝 on 2019-05-31.
//

#ifndef TIGER_CC_FILE_READER_H
#define TIGER_CC_FILE_READER_H

#include "error.h"

#include <cstdio>
#include <string>

class FileReader {
public:
    explicit FileReader(std::string &&path) {
        file_ = fopen(path.c_str(), "r");
        if (file_ == nullptr) {
            std::string err_msg;
            err_msg += "open file fail, ";
            err_msg += "filename: " + path;
            PANIC(err_msg.c_str());
        }
    }

    std::string ReadAll() {
        std::string result;
        char buf[1024];
        while (!feof(file_)) {
            memset((void*)buf, 0, 1024);
            fgets(buf, 1024, file_);
            result += std::string(buf);
        }
        return result;
    }

    ~FileReader() {
        fclose(file_);
    }

private:
    FILE *file_;
};

#endif //TIGER_CC_FILE_READER_H
