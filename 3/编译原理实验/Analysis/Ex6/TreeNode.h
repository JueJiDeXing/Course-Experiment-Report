

#ifndef ANALYSIS_TREENODE_H
#define ANALYSIS_TREENODE_H

#include <bits/stdc++.h>

using namespace std;

class TreeNode {
public:
    string value; // 节点名
    vector<TreeNode> children;// 子树

    TreeNode() = default;

    explicit TreeNode(const string &value) {
        this->value = value;
    }

    void putChild(const TreeNode &node) {
        children.push_back(node);
    }

    void putChild(const string &val) {
        children.emplace_back(val);
    }

    void printTree(const string &prefix, bool isLast) const {
        cout << prefix << (isLast ? "+--- " : "|--- ") << value;
        cout << endl;
        for (size_t i = 0; i < children.size(); ++i) {
            children[i].printTree(prefix + (isLast ? "     " : "|    "), i == children.size() - 1);
        }
    }
};

#endif //ANALYSIS_TREENODE_H
