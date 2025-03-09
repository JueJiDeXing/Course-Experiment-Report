
#include <bits/stdc++.h>

using namespace std;

#include "LRU.h"
#include "FIFO.h"
#include "LFU.h"
#include "mathematical.h"

int ICnt = 320;// 指令数
vector<int> ISeq;// 指令序列

int pageSize = 1 * 1024;// 页大小
int virtualCapacity = 32 * 1024;// 虚存容量
vector<int> page;// page[i]: 第i条指令存在的页号

pair<int, int> userRAM = {4, 32};// 用户内存容量4~32

double round(double num, int w) {
    long pow10 = 1;
    for (int i = 0; i < w; i++) pow10 *= 10;
    return round(num * pow10) / pow10;
}

string percentage(double num, int w = 2) {// num = 0.xxxx -> xx.xx
    ostringstream oss;
    oss << fixed << setprecision(w) << num * 100;
    string res = oss.str();
    if (res.find('.') == 1)res = "0" + res;
    return res;
}

int randInt(int a, int b) {
    return rand() % (b - a + 1) + a;
}


void test(int capacity) {
    cout << " " << capacity << "\t\t";
    PageAlgorithm *fifo = new FIFO(capacity), *lru = new LRU(capacity), *lfu = new LFU(capacity);
    for (int i: ISeq) {// 访问指令
        int pageNum = page[i];// 查询指令所在的页是否存在于内存中
        if (!fifo->get(pageNum))fifo->put(pageNum);// 不存在则调入内存,内存满则换出一页
        if (!lru->get(pageNum))lru->put(pageNum);
        if (!lfu->get(pageNum))lfu->put(pageNum);
    }
    // 统计数据
    double fifo_f = fifo->getPageFault(), lru_f = lru->getPageFault(), lfu_f = lfu->getPageFault();
    cout << fifo->missingCnt << "\t\t" << percentage(fifo_f) << "%\t\t";
    cout << lru->missingCnt << "\t\t" << percentage(lru_f) << "%\t\t";
    cout << lfu->missingCnt << "\t\t" << percentage(lfu_f) << "%\t\t";

    cout << percentage(standard(fifo_f, lru_f, lfu_f));
    cout << endl;
}

int main() {
    // (1) 生成320条随机指令序列
    srand(  time(nullptr)); // 初始化随机数种子
    while (ISeq.size() < ICnt) {
        int m = randInt(1, ICnt);
        ISeq.push_back(m);
        ISeq.push_back(randInt(1, m + 1));
        ISeq.push_back(randInt(m + 1, ICnt));
    }
    cout << "生成指令序列:";
    for (int i: ISeq) cout << i << ",";
    cout << "\b\n";

    // (2) 指令序列转换为页地址
    int pageCnt = virtualCapacity / pageSize;// 总页数 = 容量 / 页大小
    int per = ICnt / pageCnt; // 每页指令数 = 总指令数 / 总页数
    for (int i: ISeq) page.push_back(i / per);
    cout << "转换为页地址:";
    for (int i: page) cout << i << ",";
    cout << "\b\n";

    // (3) 测试
    cout << "内存块" << "\t\t" << "FIFO" << "\t\t\t\t" << "LRU" << "\t\t\t\t\t"
         << "LFU" << "\t\t\t\t" << "标准差" << endl;
    cout << "数量" << "\t\t";
    for (int i = 0; i < 3; i++) cout << "缺页(次)" << "\t缺页率\t\t";
    cout << "缺页率" << endl;
    for (int capacity = userRAM.first; capacity <= userRAM.second; capacity++) {
        test(capacity);
    }
    cout << "(0% 10% 20%)的标准差 = " << percentage(standard(0.00, 0.10, 0.20));

    return 0;
}