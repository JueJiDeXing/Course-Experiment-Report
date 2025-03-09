#ifndef OP_LFU_H
#define OP_LFU_H

#include <bits/stdc++.h>
#include "PageAlgorithm.h"

using namespace std;

class Node {
public:
    int key;
    int freq = 1; // 新页面只访问了一次
    Node *prev = nullptr, *next = nullptr;

    explicit Node(int key) : key(key) {}
};

class LFU : public PageAlgorithm {
public:
    map<int, Node *> keyToNode;// 页关键字 -> 页节点
    map<int, Node *> freqToDummy;// 频次 -> 页节点链
    int minFreq = 0;// 最少的频次
    explicit LFU(int capacity) : PageAlgorithm(capacity) {}


    void put(int key) override {
        if (keyToNode.size() == capacity) { // 移除freq最小的页面里最久不使用的
            Node *dummy = freqToDummy[minFreq];// 最小频次的页节点链
            Node *r = dummy->prev; // 尾端节点(最久不使用)
            keyToNode.erase(r->key);// 移除
            remove(r);
            free(r);
            if (dummy->prev == dummy) { // 被移空了
                freqToDummy.erase(minFreq); // 移除空链表
                // (最小频次置为1, 待会更新)
            }
        }
        Node *node = new Node(key); // 新页面放入缓存块
        keyToNode[key] = node;
        pushFront(1, node); // 访问1次, 最近访问(最上面)
        minFreq = 1;// 最小频次置为1
    }

    bool get(int key) override {
        total++;
        if (keyToNode.find(key) != keyToNode.end()) { // 命中
            Node *node = keyToNode[key];
            // 把页节点从当前频次的页节点链中移除, 放在 频次+1 的页节点链中
            remove(node);
            Node *dummy = freqToDummy[node->freq];
            if (dummy->prev == dummy) { // 频次为freq的链被移空了
                freqToDummy.erase(node->freq);
                if (minFreq == node->freq) {// freq是频次最低的, 更新最低频次
                    minFreq++;// 链一定非空 -> 刚放入的节点频次为minFreq+1
                }
            }
            node->freq++;
            pushFront(node->freq, node); // 最近访问, 放在同频次的最上面
            return true;
        } else {// 缺页
            missingCnt++;
            return false;
        }

    }


    /**
     创建一个新的双向链表
     */
    static Node *newList() {
        Node *dummy = new Node(0); // 哨兵节点
        dummy->prev = dummy;
        dummy->next = dummy;
        return dummy;
    }

    /**
     在链表头添加一个节点（把一本书放在最上面）
     */
    void pushFront(int freq, Node *x) {
        if (freqToDummy.find(freq) == freqToDummy.end()) {
            freqToDummy[freq] = newList();
        }
        Node *dummy = freqToDummy[freq];//链表头
        x->prev = dummy;
        x->next = dummy->next;
        x->prev->next = x;
        x->next->prev = x;
    }

    /**
     删除一个节点（抽出一本书）
     */
    static void remove(Node *x) {
        Node *prev = x->prev;
        Node *next = x->next;
        prev->next = next;
        next->prev = prev;
    }
};

#endif //OP_LFU_H
