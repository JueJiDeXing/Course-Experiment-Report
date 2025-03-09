

#ifndef OP_EX6UTIL_H
#define OP_EX6UTIL_H

#include <bits/stdc++.h>

#define NONE "\e[0m"
#define RED "\e[31m"
#define GREEN "\e[32m"
using namespace std;

class Util {
public:
    /**分割字符串*/
    static vector<string> split(const string &str, const string &delimiter) {
        vector<string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);

        while (end != string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }

        tokens.push_back(str.substr(start)); // 添加最后一个子串
        return tokens;
    }

    static void error(const string &info) {
        cout << RED << "[ERROR] " << info << NONE << endl;
    }

    static void info(const string &info) {
        cout << GREEN << "[INFO] " << info << NONE << endl;
    }

    static void ok() {
        cout << GREEN << "[INFO] OK" << NONE << endl;
    }

};

#endif //OP_EX6UTIL_H
