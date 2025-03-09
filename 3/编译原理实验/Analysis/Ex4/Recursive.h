
#ifndef ANALYSIS_RECURSIVE_H
#define ANALYSIS_RECURSIVE_H

#include "../CodeHelper/Header.h"
#include "../Ex2/Token.h"

class Recursive {// 递归下降处理程序
public:
    // 预处理first和follow集合
    map<string, set<string>> first, follow;
    map<string, vector<vector<string>>> source;
    string start;

    Recursive(const string &start, const map<string, vector<vector<string>>> &source) {
        this->start = start;
        this->source = source;
        first = createFirst(source);
        follow = createFollow(start, first, source);
    }

    /**
     * 根据文法创建first集合
     * @param source 文法的产生式
     * @return first集合
     */
    static map<string, set<string>> createFirst(const map<string, vector<vector<string>>> &source) {
        map<string, set<string>> first;
        bool isModify = true;
        while (isModify) {
            isModify = false;
            for (const auto &[P, rights]: source) {// Production -> [right1|...]
                for (const auto &right: rights) {
                    const set<string> &fp = selectFirst(first, source, right);
                    if (merge(first[P], fp)) {
                        isModify = true;
                    }
                }
            }
        }
        return first;
    }

    /**
     * 查询P->right的First集合
     * @param first 已计算出的First集合
     * @param source 文法产生式集合
     * @param right 一个候选项
     */
    static set<string> selectFirst(map<string, set<string>> &first,
                                   const map<string, vector<vector<string>>> &source,
                                   const vector<string> &right) {
        set<string> ans;
        bool specialEmpty = true;// 若 X->Y1Y2..Yk, 且∀i都有Yi->ε, 则ε∈First[X]
        for (const auto &r: right) {
            if (!isVn(r)) {// 终结符或ε
                ans.insert(r);
                break;
            }
            // 非终结符
            set<string> fr = first[r];
            if (haveEmpty(source.at(r), EMPTY)) {// 存在r->ε
                fr.erase(EMPTY);
                ans.insert(fr.begin(), fr.end());
            } else {// 不含ε
                ans.insert(fr.begin(), fr.end());
                specialEmpty = false;
                break;
            }
        }
        if (specialEmpty)ans.insert(EMPTY);
        return ans;
    }

    /**
     * 根据文法和first集合创建follow集合
     * @param start 文法的开始符号
     * @param first 文法的first集合
     * @param source 文法的产生式
     * @return follow集合
     */
    static map<string, set<string>> createFollow(const string &start, const map<string, set<string>> &first,
                                                 const map<string, vector<vector<string>>> &source) {
        map<string, set<string>> follow;
        follow[start].insert(END);// 开始符号的follow集合放入END
        bool isModify = true; // 是否还有要计算的follow
        while (isModify) {
            isModify = false;
            for (const auto &[P, rights]: source) {
                for (auto right: rights) { // 遍历每个候选项, 查找非终结符进行处理
                    dealRight(first, source, P, right, follow, isModify);
                }
            }
        }
        return follow;
    }

    /**
     * 处理P->right的候选项follow集合
     * @param first 计算好的first集合
     * @param source 文法产生式
     * @param P 非终结符, D->right
     * @param right 候选项, D->right
     * @param follow 当前计算出的follow集合
     * @param isModify 该方法是否修改了follow集合
     */
    static void dealRight(const map<string, set<string>> &first,
                          const map<string, vector<vector<string>>> &source,
                          const string &P,
                          const vector<string> &right,
                          map<string, set<string>> &follow,
                          bool &isModify) {
        // 正序遍历, 若pre为非终结符, 则 first[suf]- ε ∈ follow[pre]
        for (int i = 0; i < right.size() - 1; i++) {
            string pre = right[i], suf = right[i + 1];
            if (!isVn(pre))continue;
            if (isVn(suf)) {// suf为非终结符, 将first(suf)-ε加入follow(pre)
                set<string> firstSuf = first.at(suf);
                firstSuf.erase(EMPTY);
                if (merge(follow[pre], firstSuf))isModify = true;
            } else {// suf为终结符, 将suf加入follow(pre)
                if (merge(follow[pre], suf))isModify = true;
            }
        }

        string last = right.back();// 最后一个字符没有处理到
        if (isVn(last)) {
            if (merge(follow[last], follow[P]))isModify = true;
        }
        // 倒序遍历, 若pre后面均能推出空字, 则follow[D] ∈ follow[pre]
        for (int i = right.size() - 2; i >= 0; i--) {
            string pre = right[i], suf = right[i + 1];
            if (isVn(suf) && haveEmpty(source.at(suf), EMPTY)) {
                if (isVn(pre)) {
                    if (merge(follow[pre], follow[P]))isModify = true;
                }
            } else {
                break;
            }
        }
    }



    // 递归下降子程序
    int point = 0;// 字符串指针
    vector<Token> expression;// 正在判断的输入串
    string sym;//当前字符
    bool isRightExpression = true;

    void ERROR() {
        isRightExpression = false;
    }

    void advance() {
        Token &token = expression[point++];
        if (token.type == DecInt || token.type == Identifier)sym = "i";
        else sym = token.value;
    }

    bool firstContains(vector<string> right, string sym) {
        string r = right[0];
        if (isVn(r)) {
            return contains(first[r], sym);
        } else {
            return r == sym;
        }

    }

    void P(const string &p) {
        vector<vector<string>> &rights = source[p];
        bool match = false;
        for (const auto &right: rights) {
            // 查找first集合中匹配sym的候选项
            if (!firstContains(right, sym)) continue;
            match = true;

            for (const string &r: right) {// 非终结符:执行对应的程序; 终结符:匹配后执行advance
                if (isVn(r)) {// r为非终结符
                    P(r);// 递归下降
                } else {// r为终结符
                    if (sym == r)advance();// 匹配
                    else ERROR();// 无法匹配
                }
            }
            break;
        }
        if (!match) {
            if (!haveEmpty(rights, EMPTY))ERROR();// 无法推导出空字
            else if (!contains(follow[p], sym))ERROR();// 可以推导出空字, 但follow仍无法匹配
            // else 不报错
        }
    }

    /**
     * 判断表达式是否合法
     * @param e 
     * @return 
     */
    bool judge(const vector<Token> &e) {
        this->expression = e;
        point = 0;
        isRightExpression = true;
        advance();
        P(start);
        return isRightExpression && sym == END;
    }
};

#endif //ANALYSIS_RECURSIVE_H
