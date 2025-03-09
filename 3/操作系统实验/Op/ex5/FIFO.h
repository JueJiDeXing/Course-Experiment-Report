

#ifndef OP_FIFO_H
#define OP_FIFO_H

#include <bits/stdc++.h>
#include "PageAlgorithm.h"

using namespace std;

class FIFO : public PageAlgorithm {
public:
    queue<int> q;// 队列, 先进先出
    set<int> s;// 页是否存在于内存

    explicit FIFO(int capacity) : PageAlgorithm(capacity) {}

    void put(int key) override {
        if (s.size() == capacity) {// 内存已满
            int &r = q.front();// 移除队首(先进先出)
            q.pop();
            s.erase(r);// 将页面r移出内存
        }
        s.insert(key);// 将页面key加入内存
        q.push(key);
    }

    bool get(int key) override {
        total++;
        if (s.find(key) != s.end()) {// 命中
            return true;
        } else {//缺页
            missingCnt++;
            return false;
        }
    }
};

#endif //OP_FIFO_H
