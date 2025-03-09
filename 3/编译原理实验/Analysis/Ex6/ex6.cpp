#include <bits/stdc++.h>

using namespace std;

#include "../Ex1/Grammar.h"
#include "../Ex2/WordFactor.h"
#include "Recursive.h"
//#include "Recursive_.h"

int main() {
    string sourceCode = read("../Ex6/g6.txt");
    WordFactor wordFactor;
    vector<Token> tokens = wordFactor.scan(sourceCode);
    cout << "Tokens: ";
    for (const auto &tok: tokens) {
        cout << tok.value << " ";
    }
    cout << endl;

    Recursive  recursive;
    cout << "三地址码:" << endl;
    string res = recursive.convert(tokens);
    cout << res;
    return 0;
}