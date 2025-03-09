

#ifndef OP_LRU_H
#define OP_LRU_H

#include <bits/stdc++.h>

using namespace std;

#include "PageAlgorithm.h"
#include "LinkedList.h"

class LRU : public PageAlgorithm {
public:
    LinkedList linkedList;// 页节点链
    map<int, LNode *> nodeMap;// 页关键字 -> 页节点

    explicit LRU(int capacity) : PageAlgorithm(capacity) {}


    void put(int key) override {
        if (nodeMap.size() == capacity) { // 内存已满
            LNode *r = linkedList.removeLast();// 移除链表尾端(最久未使用)
            nodeMap.erase(r->key);
            free(r);
        }
        auto *node = new LNode(key);
        nodeMap[key] = node;
        linkedList.addFirst(node);
    }

    bool get(int key) override {
        total++;
        if (nodeMap.find(key) != nodeMap.end()) {  // 命中
            LNode *&pNode = nodeMap[key];
            linkedList.remove(pNode);// 更新最近使用
            linkedList.addFirst(pNode);
            return true;
        } else {// 缺页
            missingCnt++;
            return false;
        }
    }

};

#endif //OP_LRU_H
