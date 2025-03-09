

#ifndef OP_UTIL_H
#define OP_UTIL_H

#include <bits/stdc++.h>

using namespace std;

int toMinute(const pair<int, int> &timeP) {
    return (timeP.first * 60) + timeP.second;
}

pair<int, int> splitTime(const string &t) {
    size_t colonPos = t.find(':');
    int hour = stoi(t.substr(0, colonPos));
    int minute = stoi(t.substr(colonPos + 1));
    return {hour, minute};
}

int toMinute(const string &t) {
    return toMinute(splitTime(t));
}

string pre0(long h) {
    string s = to_string(h);
    if (s.length() == 1)s = "0" + s;
    return s;
}

pair<string, string> timeToString(long time) {
    return {pre0(time / 60), pre0(time % 60)};
}

double round(double num, int w) {
    long pow10 = 1;
    for (int i = 0; i < w; i++) {
        pow10 *= 10;
    }
    return round(num * pow10) / pow10;
}

#endif //OP_UTIL_H
