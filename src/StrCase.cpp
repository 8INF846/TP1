#include "StrCase.h"

StrCase::StrCase(bool isFloor, bool status): Case(isFloor, UNKNOWN_STATUS, UNKNOWN_STATUS),
        lastVisit(0), status(status) {}
StrCase::StrCase(bool isFloor, double dirtLevel, int jewelry, time_t lastVisit):
        Case(isFloor, dirtLevel, jewelry), lastVisit(lastVisit), status(true) {}
std::ostream& operator<<(std::ostream& output, const StrCase& strCase) {
    output << (strCase.status ? (strCase.isFloor ? "O" : "X") : "?");
}
