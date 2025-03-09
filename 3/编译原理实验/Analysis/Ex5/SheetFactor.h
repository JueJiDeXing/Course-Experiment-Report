

#ifndef ANALYSIS_SHEETFACTOR_H
#define ANALYSIS_SHEETFACTOR_H

#include <bits/stdc++.h>
#include "ex5Util.h"
#include "Production.h"
#include "I.h"
#include "ExtendGrammar.h"

using namespace std;

class SheetFactor {// DFA -> Action-Goto表
public:
    using Operator = pair<string, int>;// 移进Sx | 归约rx
    map<int, map<string, Operator>> ActionSheet;// 状态 + 终结符 -> 操作(sx|rx)
    map<int, map<string, int>> GotoSheet;// 状态 + 非终结符 -> 生成状态
    set<string> Vn, Vt;// 非终结符、终结符

    map<I *, map<string, I *>> dfa;// 能识别活前缀的DFA
    map<int, I *> id_I_map;// 项目集id -> 项目集
    map<string, set<string>> Follow;
    ExtendGrammar *extendGrammar;

    SheetFactor(ExtendGrammar &extendGrammar,
                const map<I *, map<string, I *>> &dfa,
                const map<int, I *> &id_I_map,
                const map<string, set<string>> &Follow) {
        Vn = extendGrammar.Vn;
        Vt = extendGrammar.Vt;
        this->dfa = dfa;
        this->id_I_map = id_I_map;
        this->Follow = Follow;
        this->extendGrammar = &extendGrammar;
        dfaToSheet();

    }

    /**
     * 通过DFA建立分析表
     */
    void dfaToSheet() {
        Vt.insert(END);
        Production acceptP{extendGrammar->startProject.order, extendGrammar->S, {extendGrammar->originStart, DOT}};
        // 待约|移进项目
        for (auto &[id, items]: id_I_map) {
            map<string, I *> edges = dfa.at(items);
            for (const auto &[edgeChar, nextI]: edges) {
                if (isVn(edgeChar)) {
                    GotoSheet[id][edgeChar] = nextI->id;
                } else {
                    ActionSheet[id][edgeChar] = {"S", nextI->id};
                }
            }
            // 接受项目
            if (items->contains(acceptP)) {
                ActionSheet[id][END] = {"acc", 0};
            }
            // 归约项目
            for (const auto &p: items->productions) {
                auto right = p.right;
                if (p.isReductionProject(DOT) && p != acceptP) {// 归约项目
                    // √ - SLR(1):  ActionSheet[id][Follow] = rx
                    for (const string &follow: Follow[p.left]) {
                        ActionSheet[id][follow] = {"r", p.order};
                    }
                }
            }
        }
    }

    void printSheet() {
        cout << "状态" << "\t\t\t\t" << " Action " << "\t\t\t\t" << "Goto" << endl;
        cout << "\t\t";
        for (const string &vt: Vt) cout << vt << "\t";
        cout << "\t";
        for (const string &vn: Vn) cout << vn << "\t";
        cout << endl;
        for (auto &[id, items]: id_I_map) {
            cout << id << "\t\t";
            auto &actions = ActionSheet[id];
            auto &gotos = GotoSheet[id];
            for (const string &vt: Vt) {
                if (actions.find(vt) != actions.end()) {
                    auto op = actions[vt];
                    cout << op.first;
                    if (op.first != "acc")cout << op.second;
                    cout << "\t";
                } else {
                    cout << "_" << "\t";
                }
            }
            cout << "\t";
            for (const string &vn: Vn) {
                if (gotos.find(vn) != gotos.end()) {
                    int nextS = gotos[vn];
                    cout << nextS << "\t";
                } else {
                    cout << "_" << "\t";
                }
            }
            cout << endl;
        }
    }
};

#endif //ANALYSIS_SHEETFACTOR_H
