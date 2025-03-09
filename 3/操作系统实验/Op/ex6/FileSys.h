

#ifndef OP_FILESYS_H
#define OP_FILESYS_H

#include <bits/stdc++.h>

using namespace std;

#include "Ex6Util.h"
#include "Folder.h"
#include "SpaceSys.h"

class FileSys {
public:
    Folder *nowpath, *root;

    void init(const string &Root = "root") {
        root = Folder::newFolder(Root, nullptr);
        nowpath = root;
        Util::info(root->getProjectPath());// 打印在项目中的路径(E:\.....\cmake-build-debug\root)
    }

    static int find(const vector<Folder *> &child, const string &name, bool isFolder) {
        for (int i = 0; i < child.size(); i++) {
            if (child[i]->isFolder() == isFolder && child[i]->name == name) {
                return i;
            }
        }
        return -1;
    }

    static int findFolder(const vector<Folder *> &child, const string &name) {
        return find(child, name, true);
    }

    static int findFile(const vector<Folder *> &child, const string &name) {
        return find(child, name, false);
    }

    /**
     * 列出当前目录下的所有文件和文件夹, 文件列出大小和占用盘块号
     */
    void ls() {
        if (nowpath->ChildFolder.empty()) {
            Util::info("当前目录下无文件");
            return;
        }
        for (auto f: nowpath->ChildFolder) {
            cout << f->name;
            if (f->isFile()) {//文件
                cout << "    " << f->size << "    ["; // 大小
                for (int b: f->block) cout << b << ","; // 占用盘块
                cout << "\b]";
            }
            cout << endl;
        }
    }

    /**
     * 创建目录, 支持多级目录, 以"/"分隔
     * @param args size=1 \<name>
     */
    void mkdir(const vector<string> &args) {
        if (args.size() != 1) {
            Util::error("参数数量" + to_string(args.size()) + "不正确");
            return;
        }
        const vector<string> &dir = Util::split(args[0], "/");// 按'/'分隔
        int idx = findFolder(nowpath->ChildFolder, dir[0]);
        if (idx != -1) {// 检查是否已存在
            Util::error("文件夹'" + dir[0] + "'已存在");
            return;
        }
        // 不存在, 创建多级目录
        Folder *pNowPath = nowpath;
        for (auto &d: dir) {
            Folder *newFolder = Folder::newFolder(d, pNowPath);
            pNowPath->ChildFolder.push_back(newFolder);
            pNowPath = newFolder;
        }
        Util::ok();
    }

    /**
     * 递归删除a的所有子文件
     * @param a 文件夹
     */
    void del(Folder *a) {
        if (!a->isFolder()) return;
        for (auto &f: a->ChildFolder) {
            del(f);
            f->destroy();
        }
        a->ChildFolder.clear();
    }

    /**
     * 移除目录, 单级目录
     * @param args size=1 \<name>
     */
    void rmdir(const vector<string> &args) {
        if (args.size() != 1) {
            Util::error("参数数量" + to_string(args.size()) + "不正确");
            return;
        }
        const string &name = args[0];
        auto &ChildFolder = nowpath->ChildFolder;
        int idx = findFolder(ChildFolder, name);
        if (idx == -1) {
            Util::error("文件夹'" + name + "'不存在");
            return;
        }
        del(ChildFolder[idx]);// 删除目录的子文件夹
        ChildFolder[idx]->destroy();// 删除目录
        ChildFolder.erase(ChildFolder.begin() + idx);// 在内存中删除
        Util::ok();
    }

    /**
     * 进入目录, 支持多级目录, 以"/"分隔, ".."可退至上级目录
     * @param args size=1 \<name>
     */
    void cd(const vector<string> &args) {
        if (args.size() != 1) {
            Util::error("参数数量'" + to_string(args.size()) + "'不正确");
            return;
        }
        const vector<string> &dir = Util::split(args[0], "/");
        Folder *pNowPath = nowpath;
        for (auto &d: dir) {
            if (d == "..") {// 回退上级目录
                pNowPath = pNowPath->parent;
            } else {// 进入子目录
                int idx = findFolder(pNowPath->ChildFolder, d);
                if (idx == -1) {
                    Util::error("文件夹'" + d + "'不存在");
                    return;
                }
                pNowPath = pNowPath->ChildFolder[idx];
            }
        }
        nowpath = pNowPath;
    }

/**
 * 创建文件, 单个文件, 可以不写文件后缀
 * @param args size=1~2 <name> [size]
 */
    void touch(const vector<string> &args) {
        if (args.empty() || args.size() > 2) {
            Util::error("参数数量" + to_string(args.size()) + "不正确");
            return;
        }
        const string &name = args[0];
        auto &ChildFile = nowpath->ChildFolder;
        int idx = findFile(ChildFile, name);
        if (idx != -1) {
            Util::error("文件'" + name + "'已存在");
            cout << "覆盖该文件?(Y/N):";
            string op;
            cin >> op;
            if (op == "N" || op == "n") {
                return;
            } else if (op == "Y" || op == "y") {
                rm({name});
            } else {
                Util::error("错误指令'" + op + "'");
                return;
            }
        }
        int size = args.size() == 2 ? stoi(args[1]) : 0;
        Folder *newfile = Folder::newFile(name, nowpath, size);
        ChildFile.push_back(newfile);
        Util::ok();
    }

    /**
     * 移除文件, 单个文件
     * @param args size=1 <name>
     */
    void rm(const vector<string> &args) {
        if (args.size() != 1) {
            Util::error("参数数量" + to_string(args.size()) + "不正确");
            return;
        }
        const string &name = args[0];
        auto &ChildFile = nowpath->ChildFolder;
        int idx = findFile(ChildFile, name);
        if (idx == -1) {
            Util::error("文件'" + name + "'不存在");
            return;
        }
        ChildFile[idx]->destroy();
        ChildFile.erase(ChildFile.begin() + idx);
        Util::ok();
    }

/**
 * 用户输入一行内容写入到文件
 */
    void write(Folder *file, const string &mode) {
        cout << "请输入内容: \n";
        string op;
        getline(cin, op);
        file->write(op, mode);
    }

    /**
     * 打开文件, -r读, -a追加写, -w覆盖写
     * @param args size=2 <name> <mode>
     */
    void open(const vector<string> &args) {
        if (args.size() != 2) {
            Util::error("参数数量" + to_string(args.size()) + "不正确");
            return;
        }
        const string &name = args[0], &mode = args[1];
        auto &ChildFile = nowpath->ChildFolder;
        int idx = findFile(ChildFile, name);
        if (idx == -1) {
            Util::error("文件'" + name + "'不存在");
            return;
        }
        if (mode == "-r") {
            cout << ChildFile[idx]->getcontext() << endl;
        } else if (mode == "-a" || mode == "-w") {
            write(ChildFile[idx], mode);
        } else {
            Util::error("参数'" + mode + "'错误");
            return;
        }
        ChildFile[idx]->close();
    }
};

#endif //OP_FILESYS_H
