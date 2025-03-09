#ifndef OP_RR_H
#define OP_RR_H
#include "controller.h"

class RR : public Controller {//轮转
public:
    int TimeSlice = 10;//时间片长度
    int curUseTimeSplice = 0;//当前已使用时间片长度
    /**
     * @return 当前时间片是否用完
     */
    bool isRunOut() const {
        return curUseTimeSplice == TimeSlice;
    }

    bool processRun(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) override {
        PCB &p = processMap[id];
        p.run(now);
        now++;
        checkProcessArrive(arriveTimeMap, now);
        curUseTimeSplice++;
        // 进程结束 或 时间片用完 -> 结束
        bool isFinish = p.isFinished()||isRunOut();

        if (p.isFinished()) {// 进程结束
            p.end = now;
            isFinish = true;
        } else if (isRunOut()) {// 进程未结束但时间片用完, 重新放回就绪队列
            readyQueue.push_back(id);
            isFinish = true;
        }
        return isFinish;
    }

    bool processFinished(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) override {
        curUseTimeSplice = 0;
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

#endif //OP_RR_H
