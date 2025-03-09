

#ifndef OP_FCFS_H
#define OP_FCFS_H

#include "controller.h"

class FCFS : public Controller {// 先来先服务
public:
    bool processRun(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) override {
        PCB &p = processMap[id];
        p.run(now);
        now++;
        checkProcessArrive(arriveTimeMap, now);
        // 进程结束 -> 结束
        bool isFinish = p.isFinished();
        if (isFinish) p.end = now;//运行结束
        return isFinish;
    }

    bool processFinished(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) override {
        if (!readyQueue.empty()) {
            id = readyQueue.front();//先来先服务
            readyQueue.erase(readyQueue.begin());
            processMap[id].selectedInfo(now);//打印日志
            return false;// 可以执行
        } else {
            now++;
            checkProcessArrive(arriveTimeMap, now);
            return true;// 无进程可执行
        }
    }
};

#endif //OP_FCFS_H
