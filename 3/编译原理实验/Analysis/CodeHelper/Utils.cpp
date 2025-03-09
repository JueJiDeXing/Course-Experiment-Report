#include "Utils.h"

/**
 * 读文件
 * @param filename 文件路径
 * @return 文件内容(如果无法打开文件则报错)
 */
string read(const string &filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("无法打开文件 " + filename);
    }
    string s((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();
    return s;
}


bool isVn(const string &s) {
    return isupper(s[0]);
}






/**
 * 子串替换
 */
string replaceAll(const string &str, const string &from, const string &to) {
    string result = str;
    size_t startPos = 0;
    while ((startPos = result.find(from, startPos)) != string::npos) {
        result.replace(startPos, from.length(), to);
        startPos += to.length(); // Move past the last replaced part
    }
    return result;
}




/**
 * 列表s是否以列表t开头
 */
bool startsWith(const vector<string> &s, const vector<string> &t) {
    if (t.size() > s.size()) {
        return false; // 如果t的长度大于s的长度，则s不可能以t开头
    }
    for (int i = 0; i < t.size(); i++) {
        if (s[i] != t[i])return false;
    }
    return true; // 提取s的前t.size()个字符并与t进行比较
}

/**
 * 删除指定索引的元素, 并添加新元素到末尾
 */
void modifyVector(vector<vector<string> > &modifyVec, const vector<int> &needDel,
                  const vector<vector<string> > &needAdd) {
    for (auto it = needDel.rbegin(); it != needDel.rend(); ++it) {
        if (*it >= 0 && *it < modifyVec.size()) {
            modifyVec.erase(modifyVec.begin() + *it);
        }
    }
    modifyVec.insert(modifyVec.end(), needAdd.begin(), needAdd.end());
}

/**
 * 将source的sub部分替换为replace
 * @param source 产生式右侧的一项
 * @param sub 一个因子
 * @param replace sub产生式右侧的全部项
 */
vector<vector<string>> replace(const vector<string> &source, const string &sub, const vector<vector<string>> &replace) {
    vector<vector<string>> result;
    int idx = 0;
    for (; idx < source.size(); idx++) {
        if (source[idx] == sub)break;
    }
    if (idx != source.size()) {
        for (const auto &r: replace) {
            vector<string> temp = source;
            temp.erase(temp.begin() + idx);
            temp.insert(temp.begin() + idx, r.begin(), r.end());
            result.push_back(temp);
        }
    }
    return result;
}

/**
 * 将source的第一项替换为replace
 * @param source 产生式右侧的一项
 * @param replace sub产生式右侧的全部项
 */
vector<vector<string>> replace(const vector<string> &source, const vector<vector<string>> &replace) {
    vector<vector<string>> result;
    for (const auto &r: replace) {
        vector<string> temp = source;
        temp.erase(temp.begin());
        temp.insert(temp.begin(), r.begin(), r.end());
        result.push_back(temp);
    }
    return result;
}

/**
 * 查找str1和str2的公共因子
 */
vector<string> commonPrefix(const vector<string> &str1, const vector<string> &str2) {
    int len = min(str1.size(), str2.size());
    for (int i = 0; i < len; ++i) {
        if (str1[i] != str2[i]) {
            return {str1.begin(), str1.begin() + i};
        }
    }
    return {str1.begin(), str1.begin() + len};
}

/**
 * 查找strs的公共因子
 */
vector<string> commonPrefix(const vector<vector<string>> &strs) {
    vector<string> prefix = strs[0];
    for (const auto &prod: strs) {
        prefix = commonPrefix(prefix, prod);
        if (prefix.empty())break;
    }
    return prefix;
}

/**
 * 产生大小大于1的子集
 * @param productions 产生式
 * @return {产生式子集1, 产生式子集2,...}, 子集={产生式1,产生式2,...}
 */
vector<vector<vector<string> >> generateSubsetsAndSort(const vector<vector<string>> &productions) {
    vector<vector<vector<string> >> subsets;
    int n = productions.size();
    for (int i = 1; i < (1 << n); ++i) {
        vector<vector<string> > subset;
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.push_back(productions[j]);
            }
        }
        if (subset.size() > 1) subsets.push_back(subset);
    }
    sort(subsets.begin(), subsets.end(),
         [](const vector<vector<string> > &a, const vector<vector<string> > &b) {
             return a.size() < b.size();
         });
    return subsets;
}

/**
 * Production->...|...|...中是否有某一产生式为空字
 * @param rights 产生式集合
 * @param EMPTY 空字
 */
bool haveEmpty(const vector<vector<string>> &rights, const string &EMPTY) {
    for (vector<string> right: rights) {
        if (right.size() == 1 && right[0] == EMPTY) return true;
    }
    return false;
}
