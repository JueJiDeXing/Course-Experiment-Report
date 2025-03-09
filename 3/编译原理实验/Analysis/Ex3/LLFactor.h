
#ifndef ANALYSIS_LLFACTOR_H
#define ANALYSIS_LLFACTOR_H

#include "../CodeHelper/Header.h"

using namespace std;

class LLFactor {
public:
    map<string, int> subscript;// 字符角标, D -> P0 -> P1 -> ...

    string getNext(string P) {
        string originP = string(1, P[0]);
        return originP + to_string(subscript[originP]++);// D'
    }

    /**
   * 消除左递归和回溯
   * @param source 原文法产生式
   * @return 消除左递归和回溯后的产生式
   */
    map<string, vector<vector<string>>> LL(vector<string> Vn, map<string, vector<vector<string>>> source) {
        for (int i = 0; i < Vn.size(); i++) {
            for (int j = 0; j < i; j++) {
                PjIntoPi(Vn[j], Vn[i], source);
            }
            remove_left_recursion(Vn[i], source); // 消除Pi的直接左递归
        }
        remove_left_gene(source);// 提取左因子(消除回溯)
        return source;
    }

    /**
     * 是否存在Pi -> Pj α
     */
    bool PiHavePj(const string &Pi, const string &Pj, map<string, vector<vector<string>>> source) {
        for (auto right: source[Pi]) {
            if (right[0] == Pj) return true;
        }
        return false;
    }

    /**
     * 如果 Pi -> Pj α, 则将Pj代入Pi的产生式
     */
    void PjIntoPi(const string &Pj, const string &Pi, map<string, vector<vector<string>>> &source) {
        // 检查Pi的产生式的开头中是否有Pj
        if (!PiHavePj(Pi, Pj, source))return;
        vector<int> needDel;// Pi的第几个产生式需要被替换
        vector<vector<string>> needAdd;// 需要替换为哪些产生式
        for (int k = 0; k < source[Pi].size(); k++) {
            vector<string> right = source[Pi][k];
            if (right[0] != Pj)continue;
            // 将Pj代入Pi的产生式
            vector<vector<string>> res = replace(right, source[Pj]);
            needDel.push_back(k);
            for (const vector<string> &r: res) needAdd.push_back(r);
        }
        // 执行替换操作
        modifyVector(source[Pi], needDel, needAdd);
    }

    // 消除直接左递归 -----------------------------------
    /**
     * P的产生式是否存在左递归
     * @param P 产生式左部的非终结符
     * @param rights 产生式右部
     */
    static bool haveLeftRecursion(const string &P, const vector<vector<string>> &rights) {
        for (auto r: rights) {
            if (r[0] == P)return true;// D -> D α   存在左递归
        }
        return false;
    }

    /**
     * 消除直接左递归
     * D -> D α | β
     *
     * D —> β D′
     * D′ —> αP′|ε
     * @param P 非终结符, 对P->...消除左递归
     * @param source 全部产生式
     */
    void remove_left_recursion(const string &P, map<string, vector<vector<string>>> &source) {
        if (!haveLeftRecursion(P, source[P]))return;

        // D -> D α | β
        vector<vector<string>> rights = source[P];
        source[P].clear();
        string PPrime = getNext(P);
        for (auto &r: rights) {
            if (r[0] == P) { // D -> D α   左递归式
                vector<string> newR(r.begin() + 1, r.end());// α
                newR.push_back(PPrime);// αP′
                source[PPrime].push_back(newR); // D′ —> αP′|ε
            } else {// D -> β   非左递归式
                r.push_back(PPrime);//βP′
                source[P].push_back(r);  // D —> β D′
            }
        }
        source[PPrime].push_back({EMPTY}); // 空串
    }

    // 消除回溯-------------------------------------------------------
    /**
     * 以prefix为公共左因子, 对P的产生式进行合并
     * D —> δβ1|δβ2|...|δβn|γ1|γ2|...|γm
     *
     * D —> δP`|γ1|γ2|...|γm
     * D` —> β1|β2|...|βn
     * @param P 非终结符
     * @param source 产生式
     * @param prefix P的产生式的公共左因子δ
     */
    void merge(const string &P, map<string, vector<vector<string>>> &source, vector<string> &prefix) {
        string PPrime = getNext(P);// D'

        vector<vector<string>> rights = source[P];
        source[P].clear();
        bool haveE = false;// D -> δ@    β为空时P'需要生成空字
        for (auto &right: rights) {
            if (startsWith(right, prefix)) {  // right含有公共左因子, 后面是β项, D' —> β1|β2|…|βn
                if (prefix.size() != right.size()) { // β放入P'的产生式   D' -> β
                    source[PPrime].emplace_back(right.begin() + prefix.size(), right.end());
                } else if (!haveE) {// D' -> @
                    haveE = true;
                    source[PPrime].push_back({EMPTY});
                }
            } else {// right不含公共左因子, 后面是γ项,  D —> δP′|γ1| γ2…|γm
                source[P].push_back(right);
            }
        }
        prefix.push_back(PPrime);// 最后放入P'到P,  D -> δP′
        source[P].push_back(prefix);
    }

    /**
     * 对P的产生式进行公共左因子提取
     * D —> δβ1|δβ2|...|δβn|γ1|γ2|...|γm
     *
     * D —> δP`|γ1|γ2|...|γm
     * D` —> β1|β2|...|βn
     * @param P 非终结符
     * @param source 产生式
     * @return 是否有公共左因子
     */
    bool doLeft(const string &P, map<string, vector<vector<string>>> &source) {
        vector<vector<vector<string>>> subsets = generateSubsetsAndSort(source[P]);// 生成子集(大小>1),从大到小排序
        // 检查子集是否有左因子可以提取
        for (auto &subset: subsets) {
            // D —> δβ1|δβ2|…|δβn|γ1|γ2|…|γm
            vector<string> prefix = commonPrefix(subset);//prefix=δ
            if (prefix.empty()) continue;// 无公共前缀
            // 可以提取
            merge(P, source, prefix);
            return true;
        }
        return false;
    }


    /**
     * 对文法的全体产生式提取左因子(消除回溯)
     * @param source 产生式
     */
    void remove_left_gene(map<string, vector<vector<string>>> &source) {
        bool change=true;
        while (change) {
            change=false;
            for (auto &[Pi, v]: source) {
                if(doLeft(Pi, source)){// 对每个产生式进行提取公共因子
                    change=true;// 是否提取了公共因子
                }
            }
        }
    }
};


#endif //ANALYSIS_LLFACTOR_H
