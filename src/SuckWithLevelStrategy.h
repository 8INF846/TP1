#ifndef SUCKWITHLEVELSTRATEGY_H
#define SUCKWITHLEVELSTRATEGY_H

#include "Strategy.h"
#include "Action.h"

class SuckWithLevelStrategy : public Strategy {
public:
    SuckWithLevelStrategy();
    Action findNextAction(const Sensors& sensors);

private:
    /*void initializeInternalMap();
    Action pickAction();
    float getScoreMove(Pos p);

    float _dirtLevelProb;
    float _jewelLevelProb;
    float _suckLevel;
    float _lowBattery;
    std::vector<std::vector<Case>> _internalMap;
    float _score;*/
};

#endif
