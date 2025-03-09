#ifndef PCB_H
#define PCB_H

#include <bits/stdc++.h>
#include "Util.h" // 时间转换工具函数
#include "Recorder.h"

using namespace std;

typedef struct PCB {
    int id;// 进程的唯一id
    int priority = 1;// 优先级
    int start;// 到达时间
    int needtime;// 需要运行时间
    int runtime = 0;// 已运行时间
    int end = -1;// 结束时间

    int getRemainTime() const {//还需要运行多少时间
        return needtime - runtime;
    }

    bool isFinished() const {// 进程是否结束
        return needtime == runtime;
    }

    void selectedInfo(int now) const {
        string hour = pre0(now / 60);
        string minute = pre0(now % 60);
        cout << "(" << hour << ":" << minute << ") 选择 \t 进程" << id <<
             "\t 已执行时长:" << runtime << "/" << needtime << endl;
    }

    /**
     * 进程运行1分钟
     */
    void run(int now) {
        runtime++;
        Recorder::record(now, id);
    }

    double getResponseRatio(int now) const {
        return 1 + ((now - start) - runtime) * 1.0 / needtime;
    }
} PCB;

#endif //PCB_H
