#include "Case.h"

Case::Case(): isFloor(false), dirtLevel(0.), jewelry(0) {}

Case::Case(bool isFloor, float dirtLevel, int jewelry):
        isFloor(isFloor), dirtLevel(dirtLevel), jewelry(jewelry) {}
