

#ifndef ANALYSIS_VN_CLASS_H
#define ANALYSIS_VN_CLASS_H

#include <bits/stdc++.h>

using namespace std;

class S {
public :
    string begin, // while的起始位置
    next, // 指示的下一段程序
    code; // 三地址码
};

class C {
public:
    string _true,// 判断为true时的程序
    _false,// 判断为false时的程序
    code;// 三地址码
};

typedef class UnName {
public:
    string code,//三地址码
    place;// 入口地址
} E, T, R, F, D;

#endif //ANALYSIS_VN_CLASS_H
