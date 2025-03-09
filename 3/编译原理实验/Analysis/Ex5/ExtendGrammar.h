

#ifndef ANALYSIS_EXTENDGRAMMAR_H
#define ANALYSIS_EXTENDGRAMMAR_H

#include <bits/stdc++.h>

#include "ex5Util.h"
#include "../Ex1/Grammar.h"

using namespace std;

class ExtendGrammar {// 拓广文法
public:
    string originStart;// 原文法开始符号
    string S;// 拓广文法开始符
    vector<Production> P;// 拓广文法产生式
    set<string> Vn, Vt;// 终结符与非终结符

    Production startProject;// 拓广文法的开始产生式
    Generator pOrderGen;// 产生式标号生成器
    explicit ExtendGrammar(const Grammar &grammar) {
        originStart = grammar.S;
        Vn = grammar.Vn;
        Vt = grammar.Vt;
        // 拓广文法
        S = originStart + "'";
        Vn.insert(S);
        startProject = {pOrderGen.get(), S, vector{originStart}};
        P.push_back(startProject);
        for (const auto &[left, rights]: grammar.P) {
            for (const auto &right: rights) {
                P.emplace_back(pOrderGen.get(), left, right);
            }
        }
    }

    /**
     * 获取第i个文法
     * @param i 索引
     */
    Production order(int i) const {
        return P[i];
    }

    /**
     * 获取产生式的标号
     * @param find 要查找的产生式
     * @return 标号, 未找到返回-1
     */
    int getOrder(const Production &find) const {
        for (auto p: P) {
            if (p.left == find.left && p.right == find.right) {
                return p.order;
            }
        }
        return -1;
    }

    void print() {
        cout << "拓广文法:" << endl;
        for (const Production &p: P) cout << p << endl;
    }

};

#endif //ANALYSIS_EXTENDGRAMMAR_H
