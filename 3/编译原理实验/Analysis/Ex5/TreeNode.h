

#ifndef ANALYSIS_TREENODE_H
#define ANALYSIS_TREENODE_H

#include <bits/stdc++.h>

#include <utility>
#include "Production.h"

using namespace std;

class TreeNode {// 语法树节点(带属性)
public:
    string name;// 终结符|非终结符
    map<string, string> attributes;// 属性
    vector<TreeNode *> children;// 子节点
    int useProductionId = -1;// 到子节点使用的产生式序号

    explicit TreeNode(string name) : name(std::move(name)) {}

    void setChildren(stack<TreeNode *> stk) {
        while (!stk.empty()) {
            children.push_back(stk.top());
            stk.pop();
        }
    }
};

#endif //ANALYSIS_TREENODE_H
