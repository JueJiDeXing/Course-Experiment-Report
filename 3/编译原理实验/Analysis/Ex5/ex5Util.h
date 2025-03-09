#ifndef ANALYSIS_EX5UTIL_H
#define ANALYSIS_EX5UTIL_H

#include "Production.h"
#include "../CodeHelper/Header.h"

/**
 * 求vn的闭包
 * @param vn 某项目中点号后为vn, 需要求闭包
 * @return  [vn -> ·???]
 */
static vector<Production> Closure(const string &DOT, const vector<Production> &extendGrammar,
                                  const string &vn, set<string> isVisited = {}) {
    vector<Production> ans;
    isVisited.insert(vn);
    for (const auto &p: extendGrammar) {
        auto left = p.left;
        auto right = p.right;
        if (left != vn)continue;// ? -> ·vn???
        right.insert(right.begin(), DOT);// vn->·right
        ans.emplace_back(p.order,vn, right);
        if (isVn(right[1])) { // right的首项为非终结符, 需要递归求闭包
            if (isVisited.find(right[1]) != isVisited.end())continue;
            const vector<Production> &closure = Closure(DOT, extendGrammar, right[1], isVisited);
            merge(&ans, closure);
        }
    }
    return ans;
}


#endif //ANALYSIS_EX5UTIL_H
