
#include <bits/stdc++.h>
#include "../CodeHelper/Utils.h"
#include "WordFactor.h"

using namespace std;

int main() {
    string sourceCode = read("../Ex2/g2.txt");
    cout << "文件内容:" << sourceCode<<endl;
    WordFactor wordFactor;
    vector<Token> tokens = wordFactor.scan(sourceCode);
    cout<<"Tokens:"<<endl;
    for (const Token &token: tokens) {
        if (token.type == Keyword || token.type == Symbol) {
            cout << "<" << token.value << ", " << "-" << ">" << endl;
        } else {
            cout << "<" << token.type << ", " << token.value << ">" << endl;
        }
    }

    return 0;
}
