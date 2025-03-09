#ifndef ANALYSIS_UTILS_H
#define ANALYSIS_UTILS_H

#include <bits/stdc++.h>

using namespace std;

/**
 * 读文件
 * @param filename 文件路径
 * @return 文件内容(如果无法打开文件则报错)
 */
string read(const string &filename);

/**
 * s是否为非终结符
 */
bool isVn(const string &s);

/**
 * vs中是否含有v
 */
template<typename T>
bool contains(const vector<T> &vs, const T &v) {
    for (auto &a: vs) {
        if (a == v)return true;
    }
    return false;
}

/**
 * vs中是否含有v
 */
template<typename T>
bool contains(const set<T> &vs, const T &v) {
    return vs.find(v) != vs.end();
}

/**
 * 合并v2到v1
 */
template<typename T>
void merge(vector<T> *v1, const vector<T> &v2) {
    for (auto v: v2) {
        if (!contains(*v1, v)) {
            v1->push_back(v);
        }
    }
}

/**
 * 合并v2到v1
 * @return v1是否有修改
 */
template<typename T>
bool merge(set<T> &v1, const T &v2) {
    int originalSize = v1.size();
    v1.insert(v2);
    return v1.size() != originalSize;
}

/**
 * 合并v2到v1
 * @return v1是否有修改
 */
template<typename T>
bool merge(set<T> &v1, const set<T> &v2) {
    int originalSize = v1.size();
    v1.insert(v2.begin(), v2.end());
    return v1.size() != originalSize;

}

/**
 * 栈反转
 */
template<typename T>
stack<T> reverseStk(stack<T> stk) {
    stack<T> ans;
    while (!stk.empty()) {
        ans.push(stk.top());
        stk.pop();
    }
    return ans;
}

/**
 * 子串替换
 */
string replaceAll(const string &str, const string &from, const string &to);

/**
 * 查找f在v中的位置
 * @return 不存在则返回-1
 */
template<typename T>
int find(const vector<T> &v, const T &f) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == f)return i;
    }
    return -1;
}

/**
 * 列表s是否以列表t开头
 */
bool startsWith(const vector<string> &s, const vector<string> &t);

/**
 * 删除指定索引的元素, 并添加新元素到末尾
 */
void modifyVector(vector<vector<string> > &modifyVec, const vector<int> &needDel,
                  const vector<vector<string> > &needAdd);

/**
 * 将source的sub部分替换为replace
 * @param source 产生式右侧的一项
 * @param sub 一个因子
 * @param replace sub产生式右侧的全部项
 */
vector<vector<string>> replace(const vector<string> &source, const string &sub, const vector<vector<string>> &replace);

/**
 * 将source的第一项替换为replace
 * @param source 产生式右侧的一项
 * @param replace sub产生式右侧的全部项
 */
vector<vector<string>> replace(const vector<string> &source, const vector<vector<string>> &replace);

/**
 * 查找str1和str2的公共因子
 */
vector<string> commonPrefix(const vector<string> &str1, const vector<string> &str2);

/**
 * 查找strs的公共因子
 */
vector<string> commonPrefix(const vector<vector<string>> &strs);

/**
 * 产生大小大于1的子集
 * @param productions 产生式
 * @return {产生式子集1, 产生式子集2,...}, 子集={产生式1,产生式2,...}
 */
vector<vector<vector<string> >> generateSubsetsAndSort(const vector<vector<string>> &productions);

/**
 * Production->...|...|...中是否有某一产生式为空字
 * @param rights 产生式集合
 * @param EMPTY 空字
 */
bool haveEmpty(const vector<vector<string>> &rights, const string &EMPTY);

#endif //ANALYSIS_UTILS_H
