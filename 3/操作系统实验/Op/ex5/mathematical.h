

#ifndef OP_MATHEMATICAL_H
#define OP_MATHEMATICAL_H

#include <bits/stdc++.h>

using namespace std;

int max(int a, int b, int c) {
    return max(a, max(b, c));
}

int min(int a, int b, int c) {
    return min(a, min(b, c));
}

int dis(int a, int b, int c) {
    return max(a, b, c) - min(a, b, c);
}

double fmax(double a, double b, double c) {
    return fmax(a, fmax(b, c));
}

double fmin(double a, double b, double c) {
    return fmin(a, fmin(b, c));
}

double dis(double a, double b, double c) {
    return fmax(a, b, c) - fmin(a, b, c);
}

double avg(double a, double b, double c) {
    return (a + b + c) / 3;
}

double pow2(double a) {
    return a * a;
}

double variance(double a, double b, double c) {
    double u = avg(a, b, c);
    return (pow2(a - u) + pow2(b - u) + pow2(c - u)) / 3;
}
double standard(double a, double b, double c) {
    return sqrt(variance(a,b,c));
}
#endif //OP_MATHEMATICAL_H
