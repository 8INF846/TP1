#ifndef MAP_H
#define MAP_H

#include "Startable.h"
#include "Pos.h"
#include "Case.h"
#include <math.h>
#include <vector>

class Map : public Startable {
public:
    /* Methods */
    virtual bool isFloor(Pos p) const = 0;
    virtual float dirtLevel(Pos p) const = 0;
    virtual int jewelry(Pos p) const = 0;
    virtual unsigned int width() const = 0;
    virtual unsigned int height() const = 0;
    virtual void setIsFloor(Pos p, bool isFloor) = 0;
    virtual void addJewel(Pos p) = 0;
    virtual void gatherJewelry(Pos p) = 0;
    virtual void addDirt(Pos p, double delta) = 0;
    virtual void suckDirt(Pos p, double delta) = 0;
    virtual void update(double delta = 0) = 0;
};

#endif
