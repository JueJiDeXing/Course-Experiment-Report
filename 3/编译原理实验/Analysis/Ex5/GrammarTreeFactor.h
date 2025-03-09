#ifndef ANALYSIS_GRAMMARTREEFACTOR_H
#define ANALYSIS_GRAMMARTREEFACTOR_H

#include <bits/stdc++.h>
#include "../CodeHelper/Header.h"
#include "TreeNode.h"
#include "../Ex2/Token.h"
#include "ExtendGrammar.h"
#include "Semantic.h"

using namespace std;

class GrammarTreeFactor {// 基于Action-Goto表对输入串归约构建语法树
public:
    using Operator = pair<string, int>;// 移进Sx | 归约rx | acc
    map<int, map<string, Operator>> Action;// 状态 + 终结符 -> 操作(sx|rx)
    map<int, map<string, int>> Goto;// 状态 + 非终结符 -> 生成状态

    GrammarTreeFactor(const map<int, map<string, Operator>> &ActionSheet,
                      const map<int, map<string, int>> &GotoSheet) {
        this->Action = ActionSheet;
        this->Goto = GotoSheet;
    }

    /**
     * 自底向上建树
     * @param extendGrammar 拓广文法
     * @param tokens 输入串
     * @return 根节点, 如果失败返回nullptr
     */
    TreeNode *creatTree(const ExtendGrammar &extendGrammar, vector<Token> tokens) {
        stack<TreeNode *> tree;// 节点栈, 用于建树
        stack<int> status; // 状态栈
        stack<string> chars; // 字符栈
        stack<Token> input; // 输入串(统一格式,用栈遍历输入串)
        // 初始入栈
        tokens.emplace_back(Keyword, END);
        status.push(0);
        chars.push(END);
        for (int i = tokens.size() - 1; i >= 0; i--) input.push(tokens[i]);


        //cout << "状态栈" << "\t\t\t" << "符号栈" << "\t\t\t" << "输入串" << "\t\t "
        //     << "操作" << "\t\t" << "说明" << endl;
        string preOperator = "准备"; // 打印信息时使用
        string preRemark;
        while (true) {// 移进-归约
            //printStk(status, true);
            //printStk(chars, true);
            //printStk(input, false);
            int S_top = status.top();
            string char_top = chars.top();
            Token &nextToken = input.top();
            string b = nextToken.type == DecInt ? "i" : nextToken.value;// 数字用i替换

            map<string, Operator> &actionMap = Action[S_top];
            if (actionMap.find(b) == actionMap.end()) {
                cout << "错误\t\t" << "Action[S" << S_top << "," << b << "]=err" << endl;
                return nullptr;
            }
            auto [type, num] = actionMap[b];
            //cout << preOperator << "\t\t" << preRemark;
            if (preOperator == "接受") return tree.top();

            if (type == "acc") {
                preOperator = "接受";
                preRemark = "";
            } else if (type == "S") {// 移进
                preOperator = "移进";
                preRemark = "";
                status.push(num);
                chars.push(b);
                input.pop();
                TreeNode *pNode = new TreeNode(b);
                if (nextToken.type == DecInt) {// i计入属性
                    pNode->attributes["val"] = nextToken.value;
                    pNode->attributes["code"] = nextToken.value;
                }
                tree.push(pNode);
            } else {// 归约
                preOperator = "归约";
                Production p = extendGrammar.order(num);// 使用的产生式为第num个
                preRemark = p.toString();
                int cnt = p.right.size();// 出栈个数
                stack<TreeNode *> children;
                for (int j = 0; j < cnt; j++) {
                    status.pop();
                    chars.pop();
                    children.push(tree.top());// 节点栈出栈的为子节点, 即将入栈的为父节点
                    tree.pop();
                }
                int &top = status.top();
                map<string, int> &gotoMap = Goto[top];
                if (gotoMap.find(p.left) == gotoMap.end()) { // Goto[S_top,Vn]->S_next
                    cout << "错误\t\t" << "Goto[S" << top << "," << p.left << "]=err" << endl;
                    return nullptr;
                }
                status.push(gotoMap[p.left]);
                chars.push(p.left);
                TreeNode *pNode = new TreeNode(p.left);
                pNode->setChildren(children);
                pNode->useProductionId = num;
                tree.push(pNode); // 生成父节点
            }
            //cout << endl;
        }
    }


    /**
     * 打印栈
     * @param stk 栈
     * @param reverse 是否要逆序打印, 顺序为栈顶->栈底
     */
    void printStk(const stack<string> &stk, bool reverse, int len = 16) {
        stack<string> print = reverse ? reverseStk(stk) : stk;

        int curLen = 0;
        while (!print.empty()) {
            const string &e = print.top();
            cout << e << " ";
            curLen += e.size() + 1;
            print.pop();
        }
        for (int i = 0; i < len - curLen; i++) {
            cout << " ";
        }
    }

    /**
     * 打印栈
     * @param stk 栈
     * @param reverse 是否要逆序打印, 顺序为栈顶->栈底
     */
    void printStk(const stack<int> &stk, bool reverse, int len = 16) {
        stack<int> print = reverse ? reverseStk(stk) : stk;

        int curLen = 0;
        while (!print.empty()) {
            int e = print.top();
            cout << e << " ";
            curLen += to_string(e).size() + 1;
            print.pop();
        }
        for (int i = 0; i < len - curLen; i++) {
            cout << " ";
        }
    }

    /**
     * 打印栈
     * @param stk 栈
     * @param reverse 是否要逆序打印, 顺序为栈顶->栈底
     */
    void printStk(const stack<Token> &stk, bool reverse, int len = 16) {
        stack<Token> print = reverse ? reverseStk(stk) : stk;

        int curLen = 0;
        while (!print.empty()) {
            const Token &e = print.top();
            cout << e.value << " ";
            curLen += e.value.size() + 1;
            print.pop();
        }
        for (int i = 0; i < len - curLen; i++) {
            cout << " ";
        }
    }

};

#endif //ANALYSIS_GRAMMARTREEFACTOR_H
