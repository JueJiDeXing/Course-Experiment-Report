#ifndef OP_SPACESYS_H
#define OP_SPACESYS_H

#include <bits/stdc++.h>
#include "Ex6Util.h"

using namespace std;

class SpaceSys {
public:
    static int blockCnt;// 盘块数
    static int blockSize;// 盘块大小
    static queue<int> blockQ;// 空闲盘块链
    /**
     * 初始化磁盘
     * @param bCnt 盘块数
     * @param bSize 盘块大小
     */
    static void init(int bCnt = 8, int bSize = 4) {
        blockCnt = bCnt;
        blockSize = bSize;
        Util::info("磁盘盘块数" + to_string(bCnt) + ", 盘块大小" + to_string(bSize));
        for (int i = 0; i < blockCnt; i++) {
            blockQ.push(i);
        }
    }

    /**
     * 分配磁盘
     * @param size 文件大小
     * @return 分配的盘块号
     */
    static vector<int> allocation(int size) {
        int cnt = max(1, (size + blockSize - 1) / blockSize);
        if (blockQ.size() < cnt) {
            Util::error("磁盘空间不足, 当前剩余空闲盘块" + to_string(blockQ.size()));
            return {};
        }
        Util::info("分配" + to_string(cnt) + "个盘块, 当前剩余空闲盘块" + to_string(blockQ.size() - cnt));
        vector<int> ans;
        for (int i = 0; i < cnt; i++) {
            ans.push_back(blockQ.front());
            blockQ.pop();
        }
        return ans;
    }

    /**
     * 文件修改, 检查空间是否足够, 空间不足则增加盘块
     * @param block 已分配给文件的盘块
     * @param size 文件当前大小
     * @return 空间不足时返回false
     */
    static bool reAlloc(vector<int> &block, int size) {
        auto need = size - block.size() * blockSize;
        if (need <= 0) return true;// 已满足

        int cnt = (need + blockSize - 1) / blockSize;
        if (blockQ.size() < cnt) {
            Util::error("磁盘空间不足, 当前剩余空闲盘块" + to_string(blockQ.size()));
            return false;
        }
        Util::info("再分配" + to_string(cnt) + "个盘块, 当前剩余空闲盘块" + to_string(blockQ.size() - cnt));
        for (int i = 0; i < cnt; i++) {
            block.push_back(blockQ.front());
            blockQ.pop();
        }
        return true;
    }

    /**
     * 释放磁盘空间
     * @param block 盘块号
     */
    static void releases(const vector<int> &block) {
        Util::info("释放" + to_string(block.size())
                   + "个盘块, 当前剩余空闲盘块" + to_string(blockQ.size() + block.size()));
        for (int i: block) blockQ.push(i);
    }
};

#endif //OP_SPACESYS_H
