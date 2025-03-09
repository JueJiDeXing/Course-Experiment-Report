
#ifndef ANALYSIS_RECURSIVE1_H
#define ANALYSIS_RECURSIVE1_H

#include "TreeNode.h"
#include "Vn_class.h"
#include "../Ex2/Token.h"
#include "ex6Util.h"
#include "../CodeHelper/SpecialString.h"

class Recursive_ {// 递归下降处理程序(去除了code属性)
public:

    Token sym;// 当前Token
    vector<Token> tokens;// 输入的Token流
    int ptr = 0;// 当前Token在Token流的索引

    void advance() {
        if (ptr >= tokens.size()) {
            cout << "seqIdx越界";
            exit(-1);
        }
        sym = tokens[ptr++];
    }

    /**
     * 解析Token流, 转换为三地址码
     * @param tokens Token流表示的一段程序
     * @return 三地址码
     */
    string convert(vector<Token> &tokens) {
        this->tokens = tokens;
        this->tokens.emplace_back(Undefined, END);// 末尾加上终结符
        S s;
        s.next = newlabel();// L0: S.next
        advance(); // 读入第一个token
        Pro_S(s); // 递归下降进行解析
        // 解析完成
        if (sym.type != Undefined) error();// 还有未解析的token
        return s.next + ":" + "\t" + colorString("//S.next", GREY);
    }

    /**
     * 检查currToken的类型是否匹配
     * @param type 匹配类型
     * @param value 一字一码时匹配值
     */
    bool matchToken(int type, const string &value = EMPTY) const {
        if (type != sym.type) return false;// 匹配类型
        if (type == Keyword || type == Symbol) return sym.value == value;// 一字一码时匹配值
        return true;
    }

    static string gen(string code) {
        if (code.back() != ':') {
            code += "\n";
        }
        return code;
    }

    static string genKeyword(const string &keyword) {
        return colorString(" " + keyword + " ", PURPLE);
    }

    TreeNode Pro_S(S &s) {
        TreeNode tree("S");
        if (matchToken(Identifier)) {// S -> id=E;
            // id
            string id = sym.value;
            advance();
            tree.putChild(id);
            // =
            if (!matchToken(Symbol, "=")) error();
            advance();
            tree.putChild("=");
            // E
            E e;
            const TreeNode &tree_e = Pro_E(e);
            tree.putChild(tree_e);
            // ;
            if (!matchToken(Symbol, ";")) error();
            advance();
            tree.putChild(";");
            // 计算code
            cout << "\t" + gen(id + " := " + e.place);
        } else if (matchToken(Keyword, "if")) { // S -> if C then S
            // if
            advance();
            tree.putChild("if");
            // C
            C c;
            c._true = newlabel();
            c._false = s.next;
            const TreeNode &tree_c = Pro_C(c);
            cout << gen(c._true + ":");
            tree.putChild(tree_c);
            // then
            if (!matchToken(Keyword, "then")) error();
            advance();
            tree.putChild("then");
            // S
            S s1;
            s1.next = s.next;
            s1.begin = c._true;
            const TreeNode &tree_s1 = Pro_S(s1);
            tree.putChild(tree_s1);
            // 计算code

        } else if (matchToken(Keyword, "while")) { //S -> while C do S1
            bool beginEmpty = s.begin.empty();
            if (beginEmpty) s.begin = newlabel();
            if (beginEmpty) cout << gen(s.begin + ":");
            // while
            advance();
            tree.putChild("while");
            // C
            C c;
            c._true = newlabel();
            c._false = s.next;
            const TreeNode &tree_c = Pro_C(c);
            cout << gen(c._true + ":");
            tree.putChild(tree_c);
            // do
            if (!matchToken(Keyword, "do"))error();
            advance();
            tree.putChild("do");
            // S
            S s1;
            s1.next = s.begin;
            const TreeNode &tree_s1 = Pro_S(s1);
            tree.putChild(tree_s1);
            cout << "\t" + gen(genKeyword("goto") + s.begin);
        }
        return tree;
    }

    TreeNode Pro_C(C &c) {// C -> E1 ? E2
        TreeNode tree("C");
        // E1
        E e1;
        const TreeNode &tree_e1 = Pro_E(e1);
        tree.putChild(tree_e1);
        if (matchToken(Symbol, ">")) { // C -> E1 > E2
            // >
            advance();
            tree.putChild(">");
            // E2
            E e2;
            const TreeNode &tree_e2 = Pro_E(e2);
            tree.putChild(tree_e2);
            // 计算code
            cout << "\t" + gen(genKeyword("if") + e1.place + " > " + e2.place
                               + genKeyword("goto") + c._true)
                    + "\t" + gen(genKeyword("goto") + c._false); // 此时c.false=占位符, 还未计算, 计算后进行回填

        } else if (matchToken(Symbol, "<")) { // C -> E1 < E2
            // <
            advance();
            tree.putChild("<");
            // E2
            E e2;
            const TreeNode &tree_e2 = Pro_E(e2);
            tree.putChild(tree_e2);
            // 计算code
            cout << "\t" + gen(genKeyword("if") + e1.place + " < " + e2.place
                               + genKeyword("goto") + c._true)
                    + "\t" + gen(genKeyword("goto") + c._false);
        } else if (matchToken(Symbol, "=")) { // C -> E1 = E2
            // =
            advance();
            tree.putChild("=");
            // E2
            E e2;
            const TreeNode &tree_e2 = Pro_E(e2);
            tree.putChild(tree_e2);
            // 计算code
            cout << "\t" + gen(genKeyword("if") + e1.place + " = " + e2.place
                               + genKeyword("goto") + c._true)
                    + "\t" + gen(genKeyword("goto") + c._false);
        } else {
            error();
        }
        return tree;
    }

    TreeNode Pro_E(E &e) { // E -> TR
        TreeNode tree("E");
        // T
        T t;
        const TreeNode &tree_t = Pro_T(t);
        tree.putChild(tree_t);
        // R
        R r;
        r.place = t.place;
        const TreeNode &tree_r = Pro_R(r);
        tree.putChild(tree_r);
        // R计算完毕, 向上传递
        e.place = r.place;
        return tree;
    }

    TreeNode Pro_R(R &r) { // R -> ? T R
        TreeNode tree("R");
        if (matchToken(Symbol, "+")) {//R -> + T1 R1
            // +
            advance();
            tree.putChild("+");
            // T
            T t1;
            const TreeNode &tree_t = Pro_T(t1);
            tree.putChild(tree_t);
            // 生成T + T1的三地址码
            string t_place = r.place;
            r.place = newtemp();
            cout << "\t" + gen(r.place + " := " + t_place + " + " + t1.place);
            // R1
            R r1 = r; // 将R的属性传递给R1, 等待R1计算完后向上传递
            const TreeNode &tree_r1 = Pro_R(r1);
            tree.putChild(tree_r1);
            // R1计算完毕, 向上传递
            r.place = r1.place;
        } else if (matchToken(Symbol, "-")) {//R -> - T1 R1
            // -
            advance();
            tree.putChild("-");
            // T
            T t1;
            const TreeNode &tree_t1 = Pro_T(t1);
            tree.putChild(tree_t1);

            string t_place = r.place;
            r.place = newtemp();
            cout << "\t" + gen(r.place + " := " + t_place + " - " + t1.place);
            // R1
            R r1 = r;
            const TreeNode &tree_r1 = Pro_R(r1);
            tree.putChild(tree_r1);
            // R1计算完毕, 向上传递
            r.place = r1.place;
        } else { // R -> ε
            tree.putChild(EMPTY);
        }
        return tree;
    }

    TreeNode Pro_T(T &t) {// T -> F D
        TreeNode tree("T");
        // F
        F f;
        const TreeNode &tree_f = Pro_F(f);
        tree.putChild(tree_f);
        // D
        D d;// D继承F
        d.place = f.place;
        const TreeNode &tree_d = Pro_D(d);
        tree.putChild(tree_d);
        // D计算完毕, 向上传递
        t.place = d.place;
        return tree;
    }

    TreeNode Pro_D(D &d) { // D -> ? F D
        TreeNode tree("D");
        if (matchToken(Symbol, "*")) {//D -> * F1 D1
            // *
            advance();
            tree.putChild("*");
            // F
            F f1;
            const TreeNode &tree_f = Pro_F(f1);
            tree.putChild(tree_f);

            string f_place = d.place;
            d.place = newtemp();
            cout << "\t" + gen(d.place + " := " + f_place + " * " + f1.place);
            // D1
            D d1 = d; //D1继承D, 等待D1计算完毕后向上传递
            const TreeNode &tree_d = Pro_D(d1);
            tree.putChild(tree_d);
            // D1计算完毕, 向上传递
            d.place = d1.place;
        } else if (matchToken(Symbol, "/")) {// D -> / F1 D1
            // /
            advance();
            tree.putChild("/");
            // F
            F f1;
            const TreeNode &tree_f = Pro_F(f1);
            tree.putChild(tree_f);

            string f_place = d.place;
            d.place = newtemp();
            cout << "\t" + gen(d.place + " := " + f_place + " / " + f1.place);
            // D1
            D d1 = d; //D1继承D, 等待D1计算完毕后向上传递
            const TreeNode &tree_d1 = Pro_D(d1);
            tree.putChild(tree_d1);
            // D1计算完毕, 向上传递
            d.place = d1.place;
        } else {  //  D -> ε
            tree.putChild("ε");
        }
        return tree;
    }

    TreeNode Pro_F(F &f) { // F -> (E)|id|int
        TreeNode tree("F");
        if (matchToken(Symbol, "(")) { // F -> (E)
            // (
            advance();
            tree.putChild("(");
            // E
            E e;
            const TreeNode &tree_e = Pro_E(e);
            tree.putChild(tree_e);
            // )
            if (!matchToken(Symbol, ")")) error();
            advance();
            tree.putChild(")");

            f.place = e.place;
        } else if (matchToken(Identifier)) {// F -> id
            // id
            string id = sym.value;
            advance();
            TreeNode tree_id("id");
            tree_id.putChild(id);
            tree.putChild(tree_id);

            f.place = id;
        } else if (matchToken(DecInt) || matchToken(OctInt)
                   || matchToken(HexInt)) {// F -> int
            // int
            string data = sym.value;
            advance();
            TreeNode tree_int("int");
            tree_int.putChild(data);
            tree.putChild(tree_int);

            f.place = data;
        } else {
            error();
        }
        return tree;
    }

    void error() const {
        cout << "无法匹配tokens[" << ptr << "]: " << sym.value << "\n";
        exit(-1);
    }
};

#endif //ANALYSIS_RECURSIVE1_H
