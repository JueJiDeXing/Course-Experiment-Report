

#ifndef OP_PAGEALGORITHM_H
#define OP_PAGEALGORITHM_H

#include <bits/stdc++.h>

using namespace std;

class PageAlgorithm {// 页面置换算法
public:
    int capacity;// 内存容量
    int total = 0, missingCnt = 0; // (统计) 总访问次数、缺页次数

    explicit PageAlgorithm(int capacity) : capacity(capacity) {}

    virtual bool get(int page) = 0;// 查询页是否在内存中

    virtual void put(int page) = 0;// 将页放入内存, 若内存满则置换

    double getPageFault() const {// 缺页率
        return missingCnt * 1.0 / total;
    }
};

#endif //OP_PAGEALGORITHM_H
