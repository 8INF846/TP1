#ifndef CASE_H
#define CASE_H

#include <ctime>

/**
 * The representation of a case
 */
struct Case {
    Case();
    Case(bool isFloor, float dirtLevel, int jewelry);
    bool isFloor;
    float dirtLevel;
    int jewelry;
};

#endif
