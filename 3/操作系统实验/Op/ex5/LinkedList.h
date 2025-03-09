

#ifndef OP_LINKEDLIST_H
#define OP_LINKEDLIST_H

class LNode {
public:
    int key;
    LNode *prev;
    LNode *next;

    LNode(int key) : key(key) {}

    LNode(int key, LNode *prev, LNode *next) : key(key), prev(prev), next(next) {}
};

class LinkedList {
public:
    LNode *head;
    LNode *tail;

    LinkedList() {
        head = new LNode(0);
        tail = new LNode(0);
        head->next = tail;
        head->prev = tail;
        tail->prev = head;
        tail->next = head;
    }

    void addFirst(LNode *node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    void remove(LNode *node) {
        LNode *prevNode = node->prev;
        LNode *nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }

    LNode *removeLast() {
        LNode *r = tail->prev;
        remove(r);
        return r;
    }
};

#endif //OP_LINKEDLIST_H
