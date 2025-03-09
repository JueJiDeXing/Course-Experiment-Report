

#ifndef ANALYSIS_DFAFACTOR_H
#define ANALYSIS_DFAFACTOR_H

#include <bits/stdc++.h>

#include "Production.h"
#include "I.h"
#include "ExtendGrammar.h"

using namespace std;

class DfaFactor {// 拓广文法 -> 项目集规范簇DFA
public:
    map<int, I *> id_I_map;// id -> 项目集规范簇
    set<I *> visitedI;// 已计算过的项目集规范簇
    map<I *, map<string, I *>> dfa;// I -> 边 -> I
    Generator I_idGen;// 项目集id生成器

    explicit DfaFactor(const ExtendGrammar &extendGrammar) {
        // 建立项目集规范簇
        createDFA(extendGrammar);
    }

    /**
     * 根据拓广文法构建DFA项目集规范簇
     */
    void createDFA(const ExtendGrammar &extendGrammar) {
        I *I0 = new I(I_idGen.get());
        id_I_map[I0->id] = I0;
        // 把S'->·S放入I0
        Production startPro = extendGrammar.startProject;
        startPro.addFirst(DOT);
        I0->insert(startPro);
        I0->merge(Closure(DOT, extendGrammar.P, extendGrammar.originStart));// S做闭包
        // Goto
        queue<I *> q;
        q.push(I0);
        while (!q.empty()) {
            I *i = q.front();
            q.pop();
            if (visitedI.find(i) != visitedI.end())continue;
            visitedI.insert(i);
            set<string> dotNext = i->getDotNext(DOT);// 可以走的边
            //cout << *i;
            for (const string &edgeChar: dotNext) {
                I *newI = i->Goto(DOT, extendGrammar.P, edgeChar);
                I *exist = findExist(newI);
                if (exist != nullptr) {// 已存在的项目集 -> 返回已存在的项目集
                    newI = exist;
                } else {// 新项目集, 分配id
                    newI->id = I_idGen.get();
                    id_I_map[newI->id] = newI;
                    q.push(newI);
                }
                dfa[i][edgeChar] = newI;
                //cout<<"走"<<edgeChar<<"连接I"<<newI->id<<endl;
            }
        }
    }

    void printDfaEdge() {
        cout << "Dfa边:\n";
        for (auto &[id, items]: id_I_map) {
            map<string, I *> &edges = dfa[items];
            for (const auto &[edgeChar, nextI]: edges) {
                cout << *items << "---" << edgeChar << "--->" << *nextI << endl;
            }
        }
    }


    I *findExist(I *newI) {
        for (auto &[id, i]: id_I_map) if (*i == *newI) return i;
        return nullptr;
    }

};

#endif //ANALYSIS_DFAFACTOR_H
