#include <bits/stdc++.h>
#include "../Ex1/Grammar.h"
#include "../Ex4/Recursive.h"
#include "ExtendGrammar.h"
#include "DfaFactor.h"
#include "SheetFactor.h"
#include "GrammarTreeFactor.h"
#include "../Ex2/WordFactor.h"
#include "Semantic.h"
#include "CalAttribute.h"
#include "../CodeHelper/Utils.h"

using namespace std;

/**
 * 翻译并计算逆波兰表达式
 * @param expression 中缀式
 * @param extendGrammar 中缀式的拓广文法
 * @param sheetFactor Action-Goto表
 * @param wordFactor 词法分析,输入串变为Token流
 * @param calAttribute 树遍历法计算属性
 */
void tranAndCal(const string &expression, const ExtendGrammar &extendGrammar, const SheetFactor &sheetFactor,
                WordFactor &wordFactor,
                CalAttribute &calAttribute) {

    vector<Token> tokens = wordFactor.scan(expression);
    // 自底向上构建语法树
    GrammarTreeFactor grammarTreeFactor(sheetFactor.ActionSheet, sheetFactor.GotoSheet);
    TreeNode *tree = grammarTreeFactor.creatTree(extendGrammar, tokens);
    if (tree == nullptr) {
        cout << "语法树构建失败" << endl;
        return;
    }
    // 树遍历法计算属性
    calAttribute.visitNode(tree);
    // 输出code属性和value属性
    cout << setw(30) << left << replaceAll(tree->attributes["code"], "||", ",");
    cout << tree->attributes["val"] << endl;
}

int main() {
    // 文法读入
    Grammar g;
    g.readFromFile("../Ex5/g5_grammar.txt");
    cout << "源文件读取:\n" << g.sourceCode << endl;
    // SLR(1)需要计算follow集
    map<string, set<string>> first = Recursive::createFirst(g.P);
    map<string, set<string>> follow = Recursive::createFollow(g.S, first, g.P);
    // 拓广文法 - 项目集规范簇DFA - 分析表
    ExtendGrammar extendGrammar(g);
    extendGrammar.print();
    DfaFactor dfaFactor(extendGrammar);
    dfaFactor.printDfaEdge();
    SheetFactor sheetFactor(extendGrammar, dfaFactor.dfa, dfaFactor.id_I_map, follow);
    sheetFactor.printSheet();

    // 语义动作读入
    Semantic semantic(extendGrammar);
    semantic.readFromFile("../Ex5/g5_semantic.txt");

    // 准备翻译并计算逆波兰表达式
    WordFactor wordFactor;// 词法分析器
    CalAttribute calAttribute(semantic, extendGrammar);// 属性计算器

    cout << "翻译并计算逆波兰表达式:\n";
    string expressions = read("../Ex5/g5_expression.txt");
    istringstream stream(expressions);
    string expression;
    cout << setw(5) << left << "" << setw(30) << left << "input"
         << setw(30) << left << "translate" << "calculate" << endl;
    int i = 0;// 第几个输入
    while (getline(stream, expression)) {
        i++;
        expression.pop_back();
        cout << setw(5) << left << ("[" + to_string(i) + "]");
        cout << setw(30) << left << expression;
        tranAndCal(expression, extendGrammar, sheetFactor, wordFactor, calAttribute);
    }
    return 0;
}