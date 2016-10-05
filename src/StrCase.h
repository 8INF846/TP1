#ifndef STR_CASE_H
#define STR_CASE_H

#define UNKNOWN_STATUS -1
#include "Case.h"
#include "Pos.h"
#include <iostream>

struct StrCase : public Case {
    time_t lastVisit;
    Pos cameFrom;
    double gScore;
    double fScore;
    bool status;

    StrCase(bool isFloor = false, bool status = false);
    StrCase(bool isFloor, double dirtLevel, int jewelry, time_t lastVisit);
    friend std::ostream& operator<<(std::ostream& output, const StrCase& strCase);
};

#endif
