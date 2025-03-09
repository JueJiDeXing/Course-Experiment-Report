

#ifndef OP_NPP_H
#define OP_NPP_H

#include "controller.h"

class NPP : public Controller {//非抢占式优先级调度
public:
    bool processRun(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) override {
        PCB &p = processMap[id];
        p.run(now);
        now++;
        checkProcessArrive(arriveTimeMap, now);
        // 进程结束 -> 结束
        bool isFinish = p.isFinished();
        if (p.isFinished()) p.end = now;//运行结束
        return isFinish;
    }

    /**
    * 获取就绪队列中剩余需要运行时间最少的进程所在索引
    * @param processMap 进程表
    */
    int minPriorityProcessIdx(map<int, PCB> &processMap) {
        auto comp = [&processMap](int a, int b) {
            return processMap.at(a).priority < processMap.at(b).priority;
        };
        auto it = std::min_element(readyQueue.begin(), readyQueue.end(), comp);
        return distance(readyQueue.begin(), it);
    }

    bool processFinished(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) override {
        if (!readyQueue.empty()) {
            //找到最高优先级的进程
            int findIdx = minPriorityProcessIdx(processMap);
            id = readyQueue[findIdx];
            readyQueue.erase(readyQueue.begin() + findIdx);

            processMap[id].selectedInfo(now);//打印日志
            return false;
        } else {
            now++;
            checkProcessArrive(arriveTimeMap, now);
            return true;
        }
    }
};

#endif //OP_NPP_H
