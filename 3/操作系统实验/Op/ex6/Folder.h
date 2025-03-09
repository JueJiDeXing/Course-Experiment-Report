#ifndef OP_FOLDER_H
#define OP_FOLDER_H

#include <bits/stdc++.h>
#include <filesystem>
#include "Ex6Util.h"
#include "SpaceSys.h"

using namespace std;
namespace fs = std::filesystem;

class Folder {
public:
    bool is_folder; // true:文件夹  false:文件
    string name; // 文件(夹)名称
    Folder *parent; // 上级目录
    string path; // 路径
    // 文件属性
    ofstream ofs; // 输出流, 用于写入文件
    ifstream ifs; // 输入流, 用于读入文件
    int size = 0; // 文件大小(实际大小)
    vector<int> block;// 文件占用盘块号(盘块数*盘块大小)
    // 文件夹属性
    vector<Folder *> ChildFolder; // 子文件(夹)
    /**
     * 创建文件(夹)
     * @param is_folder  true:文件夹  false:文件
     * @param name 文件(夹)名称
     * @param parent 上级目录
     * @param size 文件大小(实际大小)
     */
    Folder(bool is_folder, const string &name, Folder *parent, int size = 0, int allocSize = 0) {
        this->is_folder = is_folder;
        this->parent = parent;
        this->name = name;
        this->size = size;
        path = getpath();
        //cout << "path=" << path;
        if (isFolder()) {
            if (fs::exists(path)) { // 已存在, 加载子文件夹
                loadExitDir(path);
            } else {// 不存在, 创建文件夹
                fs::create_directory(path);
            }
        } else {
            if (fs::exists(path)) {// 存在的文件, 加载大小
                this->size = fs::file_size(path);
            } else {
                ofs.open(path);// 不存在的文件, 创建
                ofs.close();
            }
            if (block.empty()) {//未分配磁盘
                alloc(allocSize);
            }
        }
    }

    /**
     * 给文件分配磁盘(至少1个盘块)
     * @param allocSize 预分配大小(新文件), 若小于实际大小则提升为实际大小
     */
    void alloc(int allocSize) {
        if (isFolder()) {
            Util::error("文件夹无法分配磁盘");
            return;
        }
        if (!block.empty()) {
            Util::error("该文件已分配磁盘");
            return;
        }
        allocSize = max(this->size, allocSize);// 小于实际大小则提升
        block = SpaceSys::allocation(allocSize);// 分配磁盘空间
    }

    /**
     * 消毁文件(夹)
     */
    void destroy() {
        if (!block.empty()) SpaceSys::releases(block);// 释放磁盘
        ofs.close();// 关闭输入输出流
        ifs.close();
        fs::remove(path);
        free(this);// 释放内存
    }

    /**
     * 加载已存在的文件夹
     * @param _path 文件夹
     */
    void loadExitDir(const string &_path) {
        for (const auto &entry: fs::directory_iterator(_path)) {
            auto status = entry.status();
            string p = entry.path().string();
            string endDir = Util::split(p, "\\").back();
            if (fs::is_regular_file(status)) {// 文件
                Folder *x = newFile(endDir, this);
                ChildFolder.push_back(x);
            } else if (fs::is_directory(status)) {// 文件夹
                Folder *x = newFolder(endDir, this); // 递归子文件夹加载
                ChildFolder.push_back(x);
            }
        }
    }

    /**
     * 新文件夹
     * @param name 文件夹名
     * @param parent 父目录
     */
    static Folder *newFolder(const string &name, Folder *parent) {
        return new Folder(true, name, parent);
    }

    /**
     * 新文件
     * @param name 文件名 
     * @param parent 父目录
     * @param allocSize 预分配大小
     */
    static Folder *newFile(const string &name, Folder *parent, int allocSize = 0) {
        return new Folder(false, name, parent, 0, allocSize);
    }


    void write(const string &s, const string &mode) {
        if (isFolder()) {
            Util::error("文件夹无法写入");
            return;
        }
        if (mode == "-a") {
            ofs.open(path, ios::app);
        } else if (mode == "-w") {
            ofs.open(path);
        } else {
            Util::error("不支持的文件写入方式'" + mode + "'");
        }
        if (SpaceSys::reAlloc(block, size + s.size())) { // 尝试分配空间
            size += s.size();
            ofs.write(s.c_str(), s.size());
        } else {
            cout << "磁盘空间不足, 无法写入" << endl;
        }
    }


    void close() {
        if (isFolder()) {
            cout << "!不是文件, 无法打开" << endl;
            return;
        }
        ofs.close();
        ifs.close();
    }

    string getpath(const string &split = "/") {//获取路径
        string ans = this->name;
        Folder *now = this;
        while (now->parent != nullptr) {// 追溯到根目录
            now = now->parent;
            ans = now->name + split + ans;
        }
        return ans;
    }

    string getProjectPath() {//获取本地磁盘路径
        return fs::current_path().string() + "\\" + getpath("\\");
    }

    string getcontext() {
        ifs.open(path);
        string ans;
        string line;
        while (getline(ifs, line)) {
            ans += "\n" + line;
        }
        return ans;
    }

    bool isFolder() const {
        return is_folder;
    }

    bool isFile() const {
        return !is_folder;
    }
};


#endif //OP_FOLDER_H
