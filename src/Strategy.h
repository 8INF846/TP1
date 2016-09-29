#ifndef STRATEGY_H
#define STRATEGY_H

#include "MapReal.h"
#include "Pos.h"
#include "Action.h"
#include "Sensors.h"

class Strategy {
public:
    virtual void reset() = 0;
    virtual Action findNextAction(const Sensors& sensors) = 0;

protected:
    float _energy;
};

#endif