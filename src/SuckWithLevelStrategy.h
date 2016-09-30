#ifndef SUCKWITHLEVELSTRATEGY_H
#define SUCKWITHLEVELSTRATEGY_H

#include "Strategy.h"
#include "Action.h"
#include "Pos.h"

#include <deque>

class SuckWithLevelStrategy : public Strategy {
public:
    SuckWithLevelStrategy();
    Action findNextAction(const Sensors& sensors);

private:
    void updateInternalMap(const Sensors& sensors);
    void printInternalMap(Pos oldest);

    std::deque<std::deque<Case>> m_internalMap;
    Pos m_basePos;
    Pos m_currentPos;


    float getScoreDiscoverCase();
    float getScoreIddle();
};

#endif
