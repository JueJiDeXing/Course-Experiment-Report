
#ifndef VectorPrint_H
#define VectorPrint_H

#include <bits/stdc++.h>

using namespace std;

void printVector(const vector<int> &vec, const string &pre, const string &split, const string &suff) {
    cout << pre;
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i != vec.size() - 1)cout << split;
    }
    cout << suff;
}


#endif //VectorPrint_H
