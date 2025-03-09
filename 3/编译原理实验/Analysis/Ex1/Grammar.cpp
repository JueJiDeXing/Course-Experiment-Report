#include "Grammar.h"

Grammar::Grammar() = default;

void Grammar::scan(const string &input) {
    istringstream stream(input);
    string line;

    while (getline(stream, line)) {// 按行分隔
        if (!line.empty() && (line.back() == '\n' || line.back() == ';')) line.pop_back();
        // 按->分隔
        size_t arrowPos = line.find("->");
        if (arrowPos == string::npos) continue;
        string leftPart = line.substr(0, arrowPos), rightPart = line.substr(arrowPos + 2);
        if (S.empty())S = leftPart;
        addToSet(Vn, Vt, leftPart);// 记录Vn、Vt
        // 产生式右侧按|分隔
        istringstream rightStream(rightPart);
        string option;
        while (getline(rightStream, option, '|')) {
            vector<string> r; // 拆分option中的每一项到vector中, 方便区分
            for (char ch: option) r.emplace_back(1, ch);
            P[leftPart].push_back(r);
            addToSet(Vn, Vt, option);
        }
    }
}

/**
 * 大写放Vn,其余放Vt
 */
void Grammar::addToSet(set<string> &Vn, set<string> &Vt, const string &s) {
    for (char c: s) {
        string x(1, c);
        if (isupper(c)) {
            Vn.insert(x);
        } else {
            Vt.insert(x);
        }
    }
}

void Grammar::readFromFile(const string &filename) {
    sourceCode = read(filename);
    scan(sourceCode);
}

vector<string> Grammar::getVn() const {
    return {Vn.begin(), Vn.end()};
}

vector<string> Grammar::getVt() const {
    return {Vt.begin(), Vt.end()};
}

void Grammar::printSourceCode() const {
    cout << "文件文本:\n" << sourceCode << endl;
}

void Grammar::printVn() const {
    cout << "非终结符:{";
    for (const string &vn: Vn) cout << vn << ",";
    if (!Vn.empty()) cout << "\b";
    cout << "}\n";
}

void Grammar::printVt() const {
    cout << "终结符:{";
    for (const string &vn: Vt) cout << vn << ",";
    if (!Vt.empty()) cout << "\b";
    cout << "}\n";
}

void Grammar::printS() const {
    cout << "开始符号:" << S << "\n";
}

void Grammar::printP() const {
    cout << "产生式:{" << "\n";
    cout << "\t";
    cout << S << "->";
    for (size_t i = 0; i < P.at(S).size(); ++i) {
        if (i > 0)cout << "|";
        for (const string &j: P.at(S)[i])cout << j;
    }
    cout << endl;

    for (auto &it: P) {
        if (it.first == S) continue;
        cout << "\t";
        cout << it.first << "->";
        for (size_t i = 0; i < it.second.size(); ++i) {
            if (i > 0)cout << "|";
            for (const string &j: it.second[i])cout << j;

        }
        cout << endl;
    }
    cout << "}\n";
}

void Grammar::print() const {
    printSourceCode();
    printVn();
    printVt();
    printS();
    printP();
}


