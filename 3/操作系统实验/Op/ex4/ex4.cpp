#include <bits/stdc++.h>

#include "PCB.h"
#include "Controller/controller.h"
#include "Controller/FCFS.h"
#include "Controller/HRRN.h"
#include "Controller/NPP.h"
#include "Controller/NPSJF.h"
#include "Controller/PP.h"
#include "Controller/PSJF.h"
#include "Controller/RR.h"
#include "Util.h"
#include "../Utils/ColorPrint.h"

using namespace std;

int n;//测试进程数量(用户输入)
map<int, PCB> processMap;//进程id->进程PCB
map<int, int> arriveTimeMap;//到达时间->进程id
int nowT = toMinute("08:00");//定义初始时间 08:00
int endT = toMinute("12:00");//定义终止时间 12:00


void readProcessList() {
    cout << "请输入进程数量:";
    cin >> n;
    cout << "请输入" << n << "行,每行格式为:进程id 到达时间 需要运行时间 优先级" << endl;
    for (int i = 0; i < n; i++) {
        PCB p;
        cin >> p.id;
        string startT;
        cin >> startT;
        p.start = toMinute(startT);
        cin >> p.needtime >> p.priority;
        processMap[p.id] = p;
        arriveTimeMap[p.start] = p.id;
    }
}

void printInitializedData() {
    cout << BABYBLUE;
    cout << "----------初始数据----------" << endl;
    cout << "id | 到达时间 | 需要运行时间 | 优先级" << endl;
    for (auto &[id, p]: processMap) {
        string hour = pre0(p.start / 60);
        string minute = pre0(p.start % 60);
        cout << p.id << "  | " << hour << ":" << minute <<
             "   | " << p.needtime << "\t\t | " << p.priority << endl;
    }
    cout << NONE;
}

Controller *chooseMethod() {
    int method;
    cout << "请选择调度算法:" << endl;
    cout << "1. FCFS(先来先服务)" << endl;
    cout << "2. HRRN(高响应比)" << endl;
    cout << "3. NPP(非抢占式优先级)" << endl;
    cout << "4. NPSJF(非抢占式短作业优先)" << endl;
    cout << "5. PP(抢占式优先级)" << endl;
    cout << "6. PSJF(抢占式短作业优先)" << endl;
    cout << "7. RR(轮转)" << endl;
    cin >> method;
    if (method == 1) {
        return new FCFS();
    } else if (method == 2) {
        return new HRRN();
    } else if (method == 3) {
        return new NPP();
    } else if (method == 4) {
        return new NPSJF();
    } else if (method == 5) {
        return new PP();
    } else if (method == 6) {
        return new PSJF();
    } else if (method == 7) {
        return new RR();
    } else {
        cout << "ERROR: 输入错误" << endl;
        exit(-1);
    }

}

//void printSummaryData() {
//    cout << PURPLE;
//    cout << "----------结果统计----------" << endl;
//    cout << "id | 到达时间 | 需要运行时间 | 完成时间 | 周转时间 | 带权周转时间" << endl;
//    double s1 = 0, s2 = 0;
//    for (auto &[id, p]: processMap) {
//        pair<string, string> s = timeToString(p.start), e = timeToString(p.end);
//        long turnaroundTime = p.end - p.start;// 周转时间 = 等待 + 运行 = 结束 - 到达
//        cout << p.id << "  | " << s.first << ":" << s.second << "  | " << p.needtime
//             << "\t\t | " << e.first << ":" << e.second << "  | " << turnaroundTime
//             << "\t\t| " << round(turnaroundTime * 1.0 / p.needtime, 2) << endl;
//        s1 += turnaroundTime;
//        s2 += turnaroundTime * 1.0 / p.needtime;
//    }
//    cout << NONE;
//    cout << BLUE;
//    cout << "平均周转时间:" << round(s1 / n, 2) << endl;
//    cout << "平均带权周转时间:" << round(s2 / n, 2) << endl;
//    cout << NONE;
//}

void printSummaryData() {
    cout << PURPLE;
    cout << "----------结果统计----------" << endl;
    cout << "id 到达时间 需要运行时间 完成时间 周转时间 带权周转时间" << endl;
    double s1 = 0, s2 = 0;
    for (auto &[id, p]: processMap) {
        pair<string, string> s = timeToString(p.start), e = timeToString(p.end);
        long turnaroundTime = p.end - p.start;// 周转时间 = 等待 + 运行 = 结束 - 到达
        cout << p.id << " " << s.first << ":" << s.second << " " << p.needtime
             << " " << e.first << ":" << e.second << " " << turnaroundTime
             << " " << round(turnaroundTime * 1.0 / p.needtime, 2) << endl;

    }
    cout << NONE;
    cout << BLUE;
    cout << "平均周转时间: (";
    for (auto &[id, p]: processMap) {
        long turnaroundTime = p.end - p.start;// 周转时间 = 等待 + 运行 = 结束 - 到达
        s1 += turnaroundTime;
        cout << turnaroundTime << "+";
    }
    cout << "\b)/5 = " << round(s1 / n, 2) << endl;

    cout << "平均带权周转时间: (";
    for (auto &[id, p]: processMap) {
        long turnaroundTime = p.end - p.start;// 周转时间 = 等待 + 运行 = 结束 - 到达
        double d = turnaroundTime * 1.0 / p.needtime;
        cout << round(d, 2) << "+";
        s2 += d;
    }

    cout << "\b)/5 = " << round(s2 / n, 2) << endl;
    cout << NONE;
}

void printProcessRun(int startTime, int endTime, int preId) {
    const pair<string, string> &s = timeToString(startTime);
    const pair<string, string> &e = timeToString(endTime);
    cout << s.first << ":" << s.second << " 至 " << e.first << ":" << e.second << "  进程" << preId << "运行" << endl;
}

void printRunMap() {
    map<int, int> m = Recorder::runMap;
    int startTime = -1;
    int preTime = -1;
    int preId = -1;
    for (auto [time, pid]: m) {
        if (preId == pid && preTime + 1 == time) {// 合并
            preTime = time;
        } else {
            if (startTime == -1) {// 初始->有
                startTime = time;
                preTime = time;
                preId = pid;
            } else {//pa->pb
                printProcessRun(startTime, preTime + 1, preId);
                startTime = time;
                preTime = time;
                preId = pid;
            }
        }
    }
    printProcessRun(startTime, preTime + 1, preId);
}

/*
5
id 到达时间 需要运行时间 优先级
0 08:00 25 4
1 08:10 20 2
2 08:25 10 1
3 08:30 20 5
4 08:35 15 3
 */
int main() {
    readProcessList();// 数据读入
    printInitializedData();

    Controller *controller = chooseMethod();
    cout << YELLOW;
    cout << "----------开始调度----------" << endl;
    controller->control(processMap, arriveTimeMap, nowT, endT);
    cout << NONE;

    printSummaryData();// 统计
    printRunMap();// cpu占用信息
    return 0;
}