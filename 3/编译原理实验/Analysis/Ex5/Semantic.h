

#ifndef ANALYSIS_SEMANTIC_H
#define ANALYSIS_SEMANTIC_H

#include <bits/stdc++.h>
#include "ExtendGrammar.h"
#include "Production.h"

using namespace std;

class Semantic {// 语义
public:
    ExtendGrammar *extendGrammar;
    vector<Production> P;// 原文法产生式
    vector<Production> semanticP;// 带下标的文法产生式
    vector<vector<string>> action;// 语义动作

    Semantic(ExtendGrammar &extendGrammar) {
        this->extendGrammar = &extendGrammar;
        this->P = extendGrammar.P;
        semanticP.reserve(P.size());
        semanticP.resize(P.size());
        action.reserve(P.size());
        action.resize(P.size());
    }

    void readFromFile(const string &filename) {// 读取输入文本
        string sourceCode = read(filename);
        vector<vector<string>> result = parse(sourceCode);// 解析语义动作文本
        for (auto &line: result) {// line = [带标号的产生式,语义动作1,语义动作2,...]
            string production = line[0];
            Production sp = parseProduction(production);// 带标号的产生式
            Production p = parseProduction(removeVnNumbers(production));// 不带标号的产生式
            sp.order = extendGrammar->getOrder(p);// 通过不带标号的产生式查找拓广文法中产生式序号
            if (sp.order == -1) {
                cout << "产生式" << sp << "不存在, 请检查输入文件是否正确" << endl;
                exit(-1);
            }
            semanticP[sp.order] = sp; // 产生式序号 -> 产生式|语义
            for (int i = 1; i < line.size(); i++) {
                action[sp.order].push_back(line[i]);
            }
        }
    }

    /**
     * 解析产生式-语义动作的输入文本
     * @param grammarSemantic 产生式-语义动作,
     * 格式为"带标号的产生式,语义动作1,语义动作2,...\n...",
     * 例如"F->i,F.code=i.val,F.val=i.val"
     * @return [[带标号的产生式,语义动作1,语义动作2,...],[...],...]
     */
    vector<vector<string>> parse(const string &grammarSemantic) {
        vector<vector<string>> result;
        istringstream stream(grammarSemantic);
        string line;

        while (getline(stream, line)) {
            istringstream lineStream(line);
            string part;
            vector<string> lineParts;

            while (getline(lineStream, part, ',')) {
                lineParts.push_back(part);
            }

            result.push_back(lineParts);
        }
        return result;
    }

    static string removeVnNumbers(const string &s) {
        regex pattern("([A-Z])\\d+"); // 匹配大写字母后跟的数字
        return regex_replace(s, pattern, "$1");   // 使用正则表达式替换匹配的部分为空字符串
    }

    /**
     * 产生式文本解析为产生式对象
     */
    Production parseProduction(string production) {
        int pos = production.find("->");
        string left = production.substr(0, pos);
        string r = production.substr(pos + 2);
        vector<string> right = splitRightPart(r);
        return Production(left, right);
    }

    /**
     * 解析产生式右部
     */
    vector<string> splitRightPart(const string &right) {// E1+T ---> E1,+,T
        vector<string> result;
        string temp;
        for (char ch: right) {
            if (isalpha(ch)) {
                if (!temp.empty()) {
                    result.push_back(temp);
                    temp.clear();
                }
                temp += ch;
            } else if (isdigit(ch)) {
                temp += ch;
            } else {
                if (!temp.empty()) {
                    result.push_back(temp);
                    temp.clear();
                }
                result.push_back(string(1, ch));
            }
        }
        if (!temp.empty()) result.push_back(temp);
        return result;
    }

    /**
     * 对语义动作进行解析
     * @param a 语义动作
     * @return [ < 符号, 属性名 > ]
     */
    static vector<pair<string, string>> analysisAction(const string &input) {
        vector<pair<string, string>> result;
        regex pattern(R"((\w+)\.(\w+))");
        smatch match;
        string::const_iterator searchStart(input.cbegin());

        while (regex_search(searchStart, input.cend(), match, pattern)) {
            result.emplace_back(match[1], match[2]);
            searchStart = match.suffix().first;
        }

        return result;
    }

    /**
     * 参数替换
     * @param a 原字符串
     * @param args 替换参数
     * @return a="a||b||c"  args=[1,2,3] -> "1||2||3"
     */
    static string replace_with_args(const string &a, const vector<string> &args) {
        stringstream ss(a);
        string segment;
        vector<string> segments;

        while (getline(ss, segment, '|')) {
            if (!segment.empty()) segments.push_back(segment);
        }

        string result;
        for (size_t i = 0; i < segments.size(); ++i) {
            if (i > 0) result += "||";
            if (i < args.size()) result += args[i];
            else result += segments[i];
        }

        return result;
    }

    /**
     * 计算属性
     * @param a  语义动作
     * @param args  右侧参数
     * @return 计算结果
     */
    string calAction(string a, vector<string> args) {
        int idx = a.find('=');
        a = a.substr(idx + 1); // 例E0.code=E1.code||T.code||+  --->  E1.code||T.code||+
        if (a.find("||") != string::npos) {// 连接(连接符一并返回)
            return replace_with_args(a, args);// 例E1.code||T.code||+  --->  1||2||+
        }
        // +或*
        if (a.find('+') != string::npos) {// 例E.val+T.val  ---> 1+2=3
            int sum = 0;
            for (auto &i: args)sum += stoi(i);
            return to_string(sum);
        }
        if (a.find('*') != string::npos) {// 例T.val*F.val  ---> 3*3=9
            int sum = 1;
            for (auto &i: args)sum *= stoi(i);
            return to_string(sum);
        }
        //赋值 例 F.val = i.val ---> i.val
        return args[0];
    }


};

#endif //ANALYSIS_SEMANTIC_H
