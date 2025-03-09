

#ifndef ANALYSIS_TOKEN_H
#define ANALYSIS_TOKEN_H

#include <bits/stdc++.h>
#include "TokenType.h"

using namespace std;

class Token {
public:
    TokenType type;
    string value;

    Token(TokenType type, const string &value) : type(type), value(value) {}

    Token() : type(Undefined), value("") {}// 默认未定义类型

    bool operator==(const Token &rhs) const {
        return type == rhs.type && value == rhs.value;
    }

    bool operator!=(const Token &rhs) const {
        return !(rhs == *this);
    }
};

#endif //ANALYSIS_TOKEN_H
