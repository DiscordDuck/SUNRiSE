#include <iostream>
#include <time.h>
#include <chrono>
#include "drivers.h"
using namespace std;
using namespace std::chrono;

using mstime = high_resolution_clock;

void pause(int seconds) {
    if (!RELEASE_TARGET) return;
    int t = time(0) + seconds;
    do { }
    while (time(0) < t);
}

void print(string str, int t) {
    cout << str;
    if (t > 0 && RELEASE_TARGET) pause(t);
}

auto timeSince(mstime::time_point start) {
    mstime::time_point now = mstime::now();
    return duration_cast<milliseconds>(now - start).count();
}

void type(string str) {
    int length = str.length(), ms = 70;
    for (int i = 0; i < length; i++) {
        auto start = mstime::now();
        if (str.at(i) == '\n') ms += 930;
        do { } while (timeSince(start) < ms);
        cout << str.at(i);
        ms = RELEASE_TARGET ? 70 : 15;
    }
}

string space(int vertical, int horizontal) {
    string str = "";
    for (int i = 0; i < vertical; i++) str += "\n";
    for (int i = 0; i < horizontal; i++) str += "\t";
    return str;
}
