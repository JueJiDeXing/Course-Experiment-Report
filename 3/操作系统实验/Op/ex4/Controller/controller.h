#ifndef OP_CONTROLLER_H
#define OP_CONTROLLER_H

#include <bits/stdc++.h>
#include "../PCB.h"

using namespace std;

class Controller {
public:
    vector<int> readyQueue;//就绪队列[进程id]
    int curRunId = -1;// 当前运行进程id
    bool isFinish = true;// 当前运行进程状态, 是否继续执行该进程
    /**
     * 进程执行
     * @param processMap 进程表
     * @param id 正在执行的进程id
     * @param  arriveTimeMap 进程创建时间表
     * @param now 当前时间
     * @return 进程是否执行完
     */
    virtual bool processRun(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) = 0;

    /**
     * 当前运行进程结束执行(调度就绪队列|等待就绪队列)
     * 调度不耗时now不变, 等待耗时now+1
     * @param processMap 进程表
     * @return id 如果就绪队列中有进程,则调度并返回调度的进程id
     * @param  arriveTimeMap 进程创建时间表
     * @param now 当前时间
     * @return 如果有进程被调度返回false(进程未执行完,可以执行), 否则返回true(进程执行完成,无进程可执行)
     */
    virtual bool processFinished(map<int, PCB> &processMap, int &id, map<int, int> &arriveTimeMap, int &now) = 0;

    /**
     * 进程调度
     * @param processMap 进程列表
     * @param  arriveTimeMap 进程到达时间->进程id
     * @param now 当前时间(初始时间)
     * @param end 结束时间
     */
    virtual void control(map<int, PCB> &processMap, map<int, int> &arriveTimeMap, int now, int end) {
        checkProcessArrive(arriveTimeMap, now);
        while (now < end) {
            if (isFinish) {// 进程运行结束(调度就绪队列|等待就绪队列)
                isFinish = processFinished(processMap, curRunId, arriveTimeMap, now);
            } else {// 进程尚未运行结束(继续运行)
                isFinish = processRun(processMap, curRunId, arriveTimeMap, now);
            }
        }
    };

    /**
     * 检查当前是否有进程到达, 如果有,则放入就绪队列
     * @param  arriveTimeMap 进程创建时间->进程id
     * @param now 当前时间
     * @return 是否有进程到达
     */
    bool checkProcessArrive(map<int, int> &arriveTimeMap, int now) {
        if (arriveTimeMap.find(now) == arriveTimeMap.end()) {//无进程到达
            return false;
        }
        readyQueue.push_back(arriveTimeMap[now]);//放入wait队列
        return true;
    }
};


#endif //OP_CONTROLLER_H
