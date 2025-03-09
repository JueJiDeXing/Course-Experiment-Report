#include "Printer.h"

/**
 * 打印first|follow集合
 */
void print(const map<string, set<string>> &s) {
    for (const auto &[k, v]: s) {
        cout << k << ": { ";
        for (const string &j: v) {
            cout << j << " ";
        }
        cout << "}\n";
    }
}

/**
 * 打印文法产生式
 * @param source 产生式
 */
void print(const map<string, vector<vector<string>>> &source) {
    for (const auto &[k, v]: source) {
        cout << k << "->";
        for (int i = 0; i < v.size(); i++) {
            for (const string &j: v[i])cout << j;
            if (i != v.size() - 1) cout << "|";
        }
        cout << "\n";
    }
}

