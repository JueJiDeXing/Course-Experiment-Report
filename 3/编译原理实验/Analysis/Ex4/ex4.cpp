#include "../Ex1/Grammar.h"
#include "../Ex3/LLFactor.h"
#include "Recursive.h"
#include "../Ex2/WordFactor.h"

using namespace std;


int main() {
    Grammar g;
    g.readFromFile("../Ex4/g4.txt");
    cout << "源文件读取:\n" << g.sourceCode << endl;

    LLFactor fac;
    cout << "转为LL(1)文法:\n";

    map<string, vector<vector<string>>> source = fac.LL(g.getVn(), g.P);
    print(source);

    cout << "递归下降程序预处理:\n";
    Recursive r(g.S, source);
    cout << "first集合:\n";
    print(r.first);
    cout << "follow集合:\n";
    print(r.follow);
    cout << "判断表达式:\n";
    string expressions = read("../Ex4/expressions.txt");
    istringstream stream(expressions);
    string expression;
    WordFactor wordFactor;
    while (getline(stream, expression)) {
        expression.pop_back();
        vector<Token> tokens = wordFactor.scan(expression);
        tokens.emplace_back(Keyword, END);
        bool isRight = r.judge(tokens);
        cout << setw(18) << left << expression << (isRight ? "正确" : "不正确") << endl;
    }
}