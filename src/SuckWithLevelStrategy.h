#ifndef SUCKWITHLEVELSTRATEGY_H
#define SUCKWITHLEVELSTRATEGY_H

#include "Strategy.h"
#include "Action.h"

#include <deque>

class SuckWithLevelStrategy : public Strategy {
public:
    SuckWithLevelStrategy();
    Action findNextAction(const Sensors& sensors);

private:
    std::deque<std::deque<Case>> _internalMap;
    /*void initializeInternalMap();
    Action pickAction();
    float getScoreMove(Pos p);

    float _dirtLevelProb;
    float _jewelLevelProb;
    float _suckLevel;
    float _lowBattery;
    float _score;*/
};

#endif
