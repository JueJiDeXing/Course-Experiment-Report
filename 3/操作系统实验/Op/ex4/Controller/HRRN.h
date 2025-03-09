#ifndef OP_HRRN_H
#define OP_HRRN_H

#include "controller.h"

class HRRN : public Controller {//高响应比优先
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

    /**
        * 获取就绪队列中最高响应比的进程所在索引
        * @param processMap 进程表
        */
    int maxRRProcessIdx(map<int, PCB> &processMap, int now) {
        auto comp = [&processMap, now](int a, int b) {
            double responseRatioA = processMap.at(a).getResponseRatio(now);
            double responseRatioB = processMap.at(b).getResponseRatio(now);
            return responseRatioA < responseRatioB;
        };

        auto it = std::max_element(readyQueue.begin(), readyQueue.end(), comp);
        return distance(readyQueue.begin(), it);
    }

    void printRR(map<int, PCB> &processMap, int now) {
        for (auto id: readyQueue) {
            PCB p = processMap[id];
            cout << "RR(" << id << ") = " << p.getResponseRatio(now) << endl;
        }
    }

    bool processFinished(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) override {
        if (!readyQueue.empty()) {
            //找到高响应比的进程
            //printRR(processMap, now);
            int findIdx = maxRRProcessIdx(processMap, now);
            id = readyQueue[findIdx];
            readyQueue.erase(readyQueue.begin() + findIdx);

            processMap[id].selectedInfo(now);
            return false;
        } else {
            now++;
            checkProcessArrive(arriveTimeMap, now);
            return true;
        }
    }

};

#endif //OP_HRRN_H
