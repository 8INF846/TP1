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
    
    float _dirtLevelProb;
    float _jewelLevelProb;
    float _suckLevel;
    std::vector<std::vector<Case>> _internalMap;
    float _score;
};

#endif
