#include <iostream>
#include <vector>
#include <memory>
#include "../Utils/VectorPrint.h"
#include "../Utils/VectorUtil.h"
#include "../Utils/ColorPrint.h"


using namespace std;
int processCount;

/**
 * 是否存在安全序列
 * @param Max 各进程的最大需求资源数, unused
 * @param Allocation 各进程的已分配资源数
 * @param Available 当前系统可用资源数
 * @return 不存在返回空数组, 存在返回安全序列
 */
vector<int> isSafe(const vector<vector<int>> &Max,
                   const vector<vector<int>> &Allocation,
                   const vector<vector<int>> &Need,
                   const vector<int> &Available) {
    vector<bool> finish(processCount, false);
    vector<int> work = Available;
    vector<int> safeSequence(processCount);
    for (int i = 0; i < processCount; i++) {
        // 查找第一个Need<work的未完成进程
        int j = 0;
        for (; j < processCount; j++) {
            if (finish[j]) continue;//该进程已完成
            if (!isAllBiggerOrEqual(work, Need[j])) continue;
            // 分配、回收、标记为已完成
            work = Add(work, Allocation[j]);
            safeSequence[i] = j;
            finish[j] = true;
            break;
        }
        if (j == processCount) return {};// 未找到
    }
    return safeSequence;
}

void printCurrSysState(const vector<vector<int>> &Max,
                       const vector<vector<int>> &Allocation,
                       const vector<vector<int>> &Need,
                       const vector<int> &Available) {
    cout << GREEN;
    cout << "当前系统资源分配情况:" << endl;
    printVector(Available, "Available: ", " ", "\n");
    cout << NONE;
    cout << RED;
    cout << "进程\t\t\t" << "MAX\t\t\t\t" << "Allocation\t\t" << "Need" << endl;
    cout << NONE;
    for (int i = 0; i < processCount; i++) {
        cout << (i % 2 == 0 ? BLUE : YELLOW);
        cout << "P" << i << "\t\t\t";
        printVector(Max[i], "", " ", "\t\t\t");
        printVector(Allocation[i], "", " ", "\t\t\t");
        printVector(Need[i], "", " ", "\n");
        cout << NONE;
    }
}


void dealRequest(vector<vector<int>> &Max, vector<vector<int>> &Allocation, vector<vector<int>> &Need,
                 vector<int> &Available, vector<int> safeSequence, int requestP, vector<int> &request) {
    cout << "P" << requestP << " 请求资源 ";
    printVector(request, "(", " ", ")\n");
    if (!isAllBiggerOrEqual(Available, request) || !isAllBiggerOrEqual(Need[requestP], request)) {
        cout << "该请求不可分配";

    } else {
        // 检查安全序列
        Allocation[requestP] = Add(Allocation[requestP], request);
        Available = Sub(Available, request);
        Need[requestP] = Sub(Need[requestP], request);
        safeSequence = isSafe(Max, Allocation, Need, Available);
        if (safeSequence.empty()) {
            cout << "分配后不存在安全序列, 不予分配" << endl;
            // 回退
            Allocation[requestP] = Sub(Allocation[requestP], request);
            Available = Add(Available, request);
            Need[requestP] = Add(Need[requestP], request);
        } else {
            printVector(safeSequence, "安全序列: P", " P", "\n");
            cout << "P" << requestP << "请求资源可分配" << endl;
        }
    }
    printCurrSysState(Max, Allocation, Need, Available);
}

int main() {
    // (1) 初始状态定义
    // TODO 改为用户输入
    vector<vector<int>> Max = {{7, 5, 3},
                               {3, 2, 2},
                               {9, 0, 2},
                               {2, 2, 2},
                               {4, 3, 3}};
    vector<vector<int>> Allocation = {{0, 1, 0},
                                      {2, 0, 0},
                                      {3, 0, 2},
                                      {2, 1, 1},
                                      {0, 0, 2}};
    vector<vector<int>> Need = Sub(Max, Allocation);
    vector<int> Available = {3, 3, 2};
    processCount = Max.size();
    printCurrSysState(Max, Allocation, Need, Available);

    // (2) 检查当状态是否安全
    vector<int> safeSequence = isSafe(Max, Allocation, Need, Available);
    if (safeSequence.empty()) {
        cout << "不存在安全序列";
        return 0;
    }
    printVector(safeSequence, "安全序列: P", " P", "\n");

    // (3) 模拟请求
    cout << UnderLine << "----------模拟请求----------" << NONE << endl;
    // TODO 改为多个请求序列
    int requestP = 0;
    vector<int> request = {3, 2, 2};
    dealRequest(Max, Allocation, Need, Available, safeSequence, requestP, request);
    return 0;
}
