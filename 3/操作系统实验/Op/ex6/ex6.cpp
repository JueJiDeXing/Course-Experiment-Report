#include <bits/stdc++.h>

using namespace std;


#include "Ex6Util.h"
#include "FileSys.h"
#include "SpaceSys.h"

FileSys fileSys;

/**
* 识别命令
* @param op 命令
* @param args 尾随参数
*/
void Cmd(const string &op, const vector<string> &args) {
    if (op == "exit") {
        exit(0);
    } else if (op == "ls") {
        fileSys.ls();
    } else if (op == "mkdir") {
        fileSys.mkdir(args);
    } else if (op == "rmdir") {
        fileSys.rmdir(args);
    } else if (op == "cd") {
        fileSys.cd(args);
    } else if (op == "touch") {
        fileSys.touch(args);
    } else if (op == "rm") {
        fileSys.rm(args);
    } else if (op == "open") {
        fileSys.open(args);
    } else {
        Util::error("非法指令'" + op + "'");
    }
}

int main() {
    SpaceSys::init(8, 4);// 8个盘块, 每个盘块大小为4 (为了演示效果,数据调小了)
    fileSys.init();
    cout << "退出系统 -- exit\n"
            "列出目录 -- ls\n"
            "创建目录 -- mkdir <name '/'>\n"
            "删除目录 -- rmdir <name '/'>\n"
            "打开目录 -- cd <name '..' '/'>\n"
            "创建文件 -- touch <name> [size]\n"
            "删除文件 -- rm <name>\n"
            "打开文件 -- open <name> <-r|-a|-w>\n";
    string s;
    while (true) {
        cout << fileSys.nowpath->getpath() << "> ";
        getline(cin, s);// 获取用户输入的指令
        const vector<string> &input = Util::split(s, " ");// 按空格拆分
        Cmd(input[0], {input.begin() + 1, input.end()});// 解析指令
    }
}
