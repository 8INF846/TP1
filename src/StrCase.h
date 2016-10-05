#ifndef STR_CASE_H
#define STR_CASE_H

#define UNKNOWN_STATUS -1
#include "Case.h"

struct StrCase : public Case {
    time_t lastVisit;

    StrCase();
    StrCase(bool isFloor, float dirtLevel, int jewelry, time_t lastVisit);
};

#endif
