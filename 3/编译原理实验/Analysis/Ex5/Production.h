

#ifndef ANALYSIS_PRODUCTION_H
#define ANALYSIS_PRODUCTION_H

#include <bits/stdc++.h>

using namespace std;

class Production {//  产生式(或项目)
public:
    int order = -1;// 产生式标号
    string left;// left -> right
    vector<string> right;

    friend ostream &operator<<(ostream &os, const Production &p) {
        os << "[" << p.order << "]" << p.left << "->";
        for (const auto &r: p.right) {
            os << r;
        }

        return os;
    }

    bool operator<(const Production &rhs) const {
        if (order < rhs.order)return true;
        if (rhs.order < order)return false;
        if (left < rhs.left)return true;
        if (rhs.left < left)return false;
        return right < rhs.right;
    }

    bool operator>(const Production &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Production &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Production &rhs) const {
        return !(*this < rhs);
    }

    Production() = default;

    Production(int order, string left, vector<string> rights) {
        this->order = order;
        this->left = std::move(left);
        this->right = std::move(rights);
    }

    Production(string left, vector<string> rights) {
        this->left = std::move(left);
        this->right = std::move(rights);
    }

    bool operator==(const Production &rhs) const {
        return order == rhs.order &&
               left == rhs.left &&
               right == rhs.right;
    }

    bool operator!=(const Production &rhs) const {
        return !(rhs == *this);
    }

    /**
     * 归约项目
     */
    bool isReductionProject(const string &DOT) const {
        return right.back() == DOT;
    }

    void addFirst(const string &s) {
        right.insert(right.begin(), s);
    }

    int findIdx(const string &find) const {
        for (int i = 0; i < right.size(); i++) {
            if (right[i] == find)return i;
        }
        return -1;
    }

    /**
     * 将right[idx]后移一位
     * @return 不改变原对象
     */
    Production moveToNext(int idx) const {
        Production p = *this;
        swap(p.right[idx], p.right[idx + 1]);
        return p;
    }

    string toString() {
        string ans = left + "->";
        for (const auto &i: right) ans += i;
        return ans;
    }
};

#endif //ANALYSIS_PRODUCTION_H
