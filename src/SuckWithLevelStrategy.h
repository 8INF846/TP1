#ifndef SUCKWITHLEVELSTRATEGY_H
#define SUCKWITHLEVELSTRATEGY_H

#include "Strategy.h"

class SuckWithLevelStrategy : public Strategy {
public:
    SuckWithLevelStrategy(MapReal& map, const Pos pBase);
    void observeAndUpdate();
    void pickAndExecAction();

private:
    void initializeInternalMap();
    Action pickAction();
    float getScoreMove(Pos p);

    float _dirtLevelProb;
    float _jewelLevelProb;
    float _suckLevel;
    float _lowBattery;
    std::vector<std::vector<Case>> _internalMap;
    float _score;
};

#endif
