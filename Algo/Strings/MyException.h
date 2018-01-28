#pragma once

#include <string>

class MyException : public std::exception {
public:
    explicit MyException(const std::string& what_) : cause(what_) {}

    const char* what() const throw() override {
        return cause.c_str();
    }

private:
    std::string cause;
};
