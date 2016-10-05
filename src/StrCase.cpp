#include "StrCase.h"

StrCase::StrCase(): Case() {}
StrCase::StrCase(bool isFloor, float dirtLevel, int jewelry, time_t lastVisit):
        Case(isFloor, dirtLevel, jewelry), lastVisit(lastVisit) {}
