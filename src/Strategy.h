#ifndef STRATEGY_H
#define STRATEGY_H

#include "MapReal.h"
#include "Pos.h"
#include "Action.h"
#include "Sensors.h"

class Strategy {
public:
    virtual Action findNextAction(const Sensors& sensors) = 0;

protected:
    float m_fenergy;
};

#endif
