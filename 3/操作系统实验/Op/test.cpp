#include<bits/stdc++.h>

using namespace std;

set<int> s1, s2;
int n;
vector mat(513, vector<int>(513));

bool check(set<int> &s, int i) {
    for (int k: s) {
        if (mat[i][k] == 0 || mat[k][i] == 0) {
            return false;
        }
    }
    return true;
}

int main() {

    cin >> n;
    s1.insert(0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> mat[i][j];
        }
    }
    for (int i = 1; i < n; i++) {
        // 检查i能否放入s
        if (check(s1, i)) {
            s1.insert(i);
        } else if (check(s2, i)) {
            s2.insert(i);
        } else {
            cout << "No";
            return 0;
        }
    }
    if (s2.size() > s1.size()) {
        swap(s1, s2);
    }
    int t = 0;
    for (int i: s1) {
        if (s1.size() - t - s2.size() <= 1)break;
        if (check(s2, i)) {
            s2.insert(i);
            t++;
        }
    }
    cout << "Yes\n" << s1.size() - t << endl;

    return 0;
}