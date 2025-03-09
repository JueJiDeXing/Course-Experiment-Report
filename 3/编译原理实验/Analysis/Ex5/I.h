

#ifndef ANALYSIS_I_H
#define ANALYSIS_I_H

#include <bits/stdc++.h>
#include <ostream>
#include "ex5Util.h"
#include "Production.h"

using namespace std;


class I {// 项目集
public:
    int id; // Ix
    set<Production> productions;// 项目

    bool operator==(const I &rhs) const {
        return productions == rhs.productions;
    }

    bool operator!=(const I &rhs) const {
        return !(rhs == *this);
    }

    I(int id) {
        this->id = id;
    }

    /**
     * 插入项目到项目集
     */
    void insert(const Production &p) {
        productions.insert(p);
    }

    /**
     * 批量插入项目到项目集
     * @param s 要插入的项目的集合
     */
    void merge(const vector<Production> &s) {
        productions.insert(s.begin(), s.end());
    }

    /**
     * 项目是否存在于当前项目集
     */
    bool contains(const Production &p) {
        return productions.find(p) != productions.end();
    }


    /**
     * 获取点号后的字符集
     */
    set<string> getDotNext(const string &DOT) {
        set<string> ans;
        for (const Production &p: productions) {
            int idx = p.findIdx(DOT);
            if (idx + 1 < p.right.size()) {
                ans.insert(p.right[idx + 1]);
            }
        }
        return ans;
    }

    friend ostream &operator<<(ostream &os, const I &i) {
        os << "I" << i.id;
        return os;
    }
    //friend ostream &operator<<(ostream &os, const I &i) {
    //    os << "I" << i.id << "{" << endl;
    //    for (const auto &p: i.productions) {
    //        os << p << endl;
    //    }
    //    os << "}" << endl;
    //    return os;
    //}

    /**
     * 走一条edgeChar边到达新的项目集
     * @param DOT 点号
     * @param source 文法产生式
     * @param edgeChar 边
     * @return 新的项目集对象, id=-1
     */
    I *Goto(const string &DOT, const vector<Production> &extendGrammar, const string &edgeChar) {
        I *newI = new I(-1);
        for (Production p: productions) {
            if (p.isReductionProject(DOT))continue;// 归约项目, 走不了
            int idx = p.findIdx(DOT);
            if (p.right[idx + 1] != edgeChar)continue;
            // 走edgeChar边
            const Production &toNext = p.moveToNext(idx);// 移动DOT后的项目
            newI->insert(toNext);
            if (idx + 2 < p.right.size()) {// DOT后有字符
                string nnext = p.right[idx + 2];
                if (isVn(nnext)) {// DOT后为非终结符, 求闭包
                    vector<Production> closure = Closure(DOT, extendGrammar, nnext);
                    newI->merge(closure);
                }
            }
        }
        return newI;
    }
};

#endif //ANALYSIS_I_H
