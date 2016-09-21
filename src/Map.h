#ifndef MAP_H
#define MAP_H

#include "Pos.h"
#include <math.h>

class Map {
public:
    // GETTERS
    virtual bool isFloor(Pos p) const = 0;
    virtual float dirtLevel(Pos p) const = 0;
    virtual int jewelry(Pos p) const = 0;
    unsigned int width();
    unsigned int height();

    static int getDistBetweenPos(Pos A, Pos B) {
        return std::abs(A.x - B.x) + std::abs(A.y - B.y);
    }
};

#endif
