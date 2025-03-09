

#ifndef ANALYSIS_WORDFACTOR_H
#define ANALYSIS_WORDFACTOR_H

#include <bits/stdc++.h>
#include "Token.h"
#include "State.h"
#include <functional>

using namespace std;


class WordFactor {// 词法分析
public:
    using P = function<State(char, string &)>;
    static map<State, P> m;

    static set<char> symbol;
    static set<State> endState;
    static set<State> backState;
    static set<string> keyword;

    WordFactor() {
        symbol = {'+', '-', '*', '/', '>', '<', '=', '(', ')', ';'};
        endState = {S2, S5, S7, S8, S10};
        backState = {S2, S5, S7, S8};
        keyword = {"if", "then", "else", "while", "do"};
        m = {
                {S0, P_S0},
                {S1, P_S1},
                {S3, P_S3},
                {S4, P_S4},
                {S6, P_S6},
                {S9, P_S9}
        };
    }

    static bool isSymbol(char ch) {
        return symbol.find(ch) != symbol.end();
    }

    static bool isEndState(State state) {
        return endState.find(state) != endState.end();
    }

    static bool isBackState(State state) {
        return backState.find(state) != backState.end();
    }

    static bool isKeyWord(string s) {
        return keyword.find(s) != keyword.end();
    }

    static State P_S0(char ch, string &buffer) {
        if (ch == ' ' || ch == '\n' || ch == '\t') {// 接收空白, 仍然为S0
            return S0;
        } else if (isalpha(ch)) {// 接收字母, 到达S1(标识符或关键字)
            buffer += ch;
            return S1;
        } else if (ch == '0') {// 接收数字0, 到达S3(十进制0或八进制或十六进制)
            buffer += ch;
            return S3;
        } else if ('1' <= ch && ch <= '9') {// 接收非0数字, 到达S9(十进制)
            buffer += ch;
            return S9;
        } else if (isSymbol(ch)) {// 接收算符和界符, 到达S10
            buffer += ch;
            return S10;
        } else {// 不可接收的字符, S11报错
            return S11;
        }
    }

    static State P_S1(char ch, string &buffer) {
        if (isalpha(ch) || isdigit(ch)) {// 接收字母或数字,仍为S1
            buffer += ch;
            return S1;
        } else {// 接受其他字符, S2回退
            return S2;
        }
    }

    static State P_S3(char ch, string &buffer) {
        if (ch == 'x') {// 接收x, 到达S4(十六进制)
            buffer += ch;
            return S4;
        } else if ('1' <= ch && ch <= '7') {// 接收1~7, 到达S6(八进制)
            buffer += ch;
            return S6;
        } else {// 其他字符, 到达S8, 识别数字0, 并回退
            return S8;
        }
    }

    static State P_S4(char ch, string &buffer) {
        if (isdigit(ch) || ('a' <= ch && ch <= 'f')) {// 接收0~f, 仍为S4(十六进制)
            buffer += ch;
            return S4;
        } else {// 其他字符, 到达S5, 识别十六进制数, 并回退
            return S5;
        }
    }

    static State P_S6(char ch, string &buffer) {
        if ('0' <= ch && ch <= '7') {// 接收0~7, 仍为S6(八进制)
            buffer += ch;
            return S6;
        } else {// 其他字符, 到达S7, 识别八进制数, 并回退
            return S7;
        }
    }

    static State P_S9(char ch, string &buffer) {
        if (isdigit(ch)) {// 接收数字, 仍为S9(十进制)
            buffer += ch;
            return S9;
        } else {// 其他字符, 到达S8, 识别十进制数, 并回退
            return S8;
        }
    }


    void deal(char ch, State &state, string &buffer) {
        if (isEndState(state)) {
            cout << "非法的状态, 应为非终结态" << endl;
            exit(-1);
        }
        state = m[state](ch, buffer);
    }


    void putToken(vector<Token> &tokens, State state, const string &buffer) {
        Token token;
        token.value = buffer;
        if (state == S2) {
            token.type = isKeyWord(buffer) ? Keyword : Identifier;
        } else if (state == S5) {
            token.type = HexInt;
        } else if (state == S7) {
            token.type = OctInt;
        } else if (state == S8) {
            token.type = DecInt;
        } else if (state == S10) {
            token.type = Symbol;
        } else {
            cout << "非法状态, 应为终结态" << endl;
            exit(-1);
        }
        tokens.push_back(token);
    }

    vector<Token> scan(string sourceCode) {
        vector<Token> tokens;// 最终结果
        string buffer;// 当前保存的字符串
        State state = S0;// 当前状态
        int i = 0;// 当前读入字符下标
        sourceCode += " ";// 末尾添加一个空白分隔
        while (i < sourceCode.size()) {
            char ch = sourceCode[i++];// 读入下一个字符
            deal(ch, state, buffer);
            if (state == S11) {
                printf("非法状态S11, 请检查源文件");
                exit(-1);
            }
            if (isBackState(state))i--;// 回退一个字符
            if (isEndState(state)) {// 终态
                putToken(tokens, state, buffer);//保存buffer中记录的token
                state = S0;// 回到初始状态
                buffer = "";
            }
        }
        return tokens;
    }
};

#endif //ANALYSIS_WORDFACTOR_H
