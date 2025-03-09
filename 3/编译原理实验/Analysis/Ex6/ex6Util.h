

#ifndef ANALYSIS_EX6UTIL_H
#define ANALYSIS_EX6UTIL_H

#include <bits/stdc++.h>

using namespace std;
#define NONE "\e[0m"
#define BLACK "\e[30m"
#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define BLUE "\e[34m"
#define PURPLE "\e[35m"
#define BABYBLUE "\e[36m"
#define GREY "\e[37m"

/**
 * 给字符串添加颜色, 打印时更好看
 * @param s 字符串
 * @param color 颜色
 */
static string colorString(const string &s, const string &color) {
    return color + s + NONE;
}


/**
    * 三地址码序号
    */
static string newlabel() {
    static int L = 0;
    return colorString("L" + to_string(L++), GREEN);
}

/**
 * 局部变量序号
 */
static string newtemp() {
    static int T = 0;
    return colorString("T" + to_string(T++), BLUE);
}


#endif //ANALYSIS_EX6UTIL_H
