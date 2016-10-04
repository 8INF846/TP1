#ifndef CASE_H
#define CASE_H

#include <ctime>

//If the dirtLevel or jewelry is unknow, we can use this value
//(useful for internal map)
#define UNKNOWN_STATUS -1

/**
 * The representation of a case
 */
struct Case {
    bool isFloor;
    float dirtLevel;
    int jewelry;
    std::time_t lastVisit;
};

#endif
