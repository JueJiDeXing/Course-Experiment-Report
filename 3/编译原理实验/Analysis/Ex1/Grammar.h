
#ifndef ANALYSIS_GRAMMAR_H
#define ANALYSIS_GRAMMAR_H

#include <bits/stdc++.h>
#include "../CodeHelper/Header.h"

using namespace std;

/*
 * 文法的读入和输出
 */
class Grammar {
public:

    explicit Grammar();

    string S;//开始符
    set<string> Vn;//非终结符
    set<string> Vt;//终结符
    map<string, vector<vector<string>>> P;// left -> right1|right2|...
    string sourceCode;// 原始文本


    /**
     * 将文本转为产生式, 存储在类对象内
     * @param input 输入的产生式文本
     */
    void scan(const string &input);

    /**
     * 根据是否大写区分非终结符和终结符
     * @param Vn 非终结符集合
     * @param Vt 终结符集合
     * @param s 字符, 如果是非终结符则放入Vn, 否则放入Vt
     */
    static void addToSet(set<string> &Vn, set<string> &Vt, const string &s);

    /**
     * 读取文件文本, 并转为产生式
     */
    void readFromFile(const string &filename);

    vector<string> getVn() const;

    vector<string> getVt() const;

    // 打印信息的方法
    void printSourceCode() const;

    void printVn() const;

    void printVt() const;

    void printS() const;

    void printP() const;

    void print() const;
};


#endif //ANALYSIS_GRAMMAR_H
