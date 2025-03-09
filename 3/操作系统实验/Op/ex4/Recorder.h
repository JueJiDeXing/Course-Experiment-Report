

#ifndef OP_RECORDER_H
#define OP_RECORDER_H

#include <bits/stdc++.h>

using namespace std;

class Recorder {
public:
    static map<int, int> runMap;

    static void record(int time, int pid) {
        runMap[time] = pid;
    }
};

#endif //OP_RECORDER_H
