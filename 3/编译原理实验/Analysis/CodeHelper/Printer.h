

#ifndef ANALYSIS_PRINTER_H
#define ANALYSIS_PRINTER_H

#include <bits/stdc++.h>

using namespace std;

/**
 * 打印first|follow集合
 */
void print(const map<string, set<string>> &s) ;

/**
 * 打印文法产生式
 * @param source 产生式
 */
void print(const map<string, vector<vector<string>>> &source) ;


#endif //ANALYSIS_PRINTER_H
