#ifndef CASE_H
#define CASE_H

#include <ctime>

#define UNKNOWN_STATUS -1

struct Case {
    bool isFloor;
    float dirtLevel;
    int jewelry;
    std::time_t lastVisit;
};

#endif
