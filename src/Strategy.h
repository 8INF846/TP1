#ifndef STRATEGY_H
#define STRATEGY_H

#include "MapReal.h"
#include "Pos.h"
#include "Action.h"
#include "Sensors.h"

class Strategy {
public:
    /**
     * Find the next Action for the Vacuum based on sensors value
     * @param sensors
     * @return the next Action to execute
     */
    virtual Action findNextAction(const Sensors& sensors) = 0;
};

#endif
