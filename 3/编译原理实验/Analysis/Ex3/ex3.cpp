#include <bits/stdc++.h>
#include "../Ex1/Grammar.h"
#include "LLFactor.h"

using namespace std;


void saveToFile(const map<string, vector<vector<string>>> &source, const string &filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "无法打开文件: " << filename << endl;
        return;
    }

    for (const auto &[k, v]: source) {
        outFile << k << "->";
        for (int i = 0; i < v.size(); i++) {
            for (const string &j: v[i]) outFile << j;
            if (i != v.size() - 1)outFile << "|";
        }
        outFile << ";\n";
    }

    outFile.close();
}

int main() {
    Grammar g;
    g.readFromFile("../Ex3/g3.txt");
    cout << "源文件读取:" << g.sourceCode << endl;

    LLFactor fac;
    cout << "转为LL(1)文法:\n";
    map<string, vector<vector<string>>> LL1 = fac.LL(g.getVn(), g.P);
    print(LL1);

    const char *savePath = "../Ex3/out.txt";
    saveToFile(LL1, savePath);
    cout << "输出文件已保存在" << savePath << endl;
    return 0;
}