#ifndef VectorUtil_H
#define VectorUtil_H

#include <bits/stdc++.h>

using namespace std;

bool isAllBiggerOrEqual(vector<int> a, vector<int> b) {
    for (int i = 0; i < a.size(); i++) {
        if (a[i] < b[i])return false;
    }
    return true;
}

vector<int> Add(vector<int> a, vector<int> b) {
    vector<int> c;
    for (int i = 0; i < a.size(); i++) {
        c.push_back(a[i] + b[i]);
    }
    return c;
}

vector<int> Sub(vector<int> a, vector<int> b) {
    vector<int> c;
    for (int i = 0; i < a.size(); i++) {
        c.push_back(a[i] - b[i]);
    }
    return c;
}

vector<vector<int>> Add(vector<vector<int>> a, vector<vector<int>> b) {
    vector<vector<int>> c;
    for (int i = 0; i < a.size(); i++) {
        c.push_back(Add(a[i], b[i]));
    }
    return c;
}

vector<vector<int>> Sub(vector<vector<int>> a, vector<vector<int>> b) {
    vector<vector<int>> c;
    for (int i = 0; i < a.size(); i++) {
        c.push_back(Sub(a[i], b[i]));
    }
    return c;
}

#endif //VectorUtil_H
