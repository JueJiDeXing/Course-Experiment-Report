

#ifndef ANALYSIS_TOKENTYPE_H
#define ANALYSIS_TOKENTYPE_H

enum TokenType {
    Undefined = -1,//未定义
    Identifier = 0, // 标识符
    DecInt = 1, // 十进制整数
    OctInt = 2, // 八进制整数
    HexInt = 3, // 十六进制整数
    Symbol = 4, //  运算符和界符
    Keyword = 5 // 关键字
};

#endif //ANALYSIS_TOKENTYPE_H
