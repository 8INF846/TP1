#ifndef SUCKWITHLEVELSTRATEGY_H
#define SUCKWITHLEVELSTRATEGY_H

#include "Strategy.h"
#include "Action.h"
#include "Pos.h"
#include "Case.h"

#include <deque>

class SuckWithLevelStrategy : public Strategy {
public:
    SuckWithLevelStrategy();
    Action findNextAction(const Sensors& sensors);

private:
    /**
     * Update m_internalMap
     * @param sensors
     */
    void updateInternalMap(const Sensors& sensors);
    /**
     * Print m_internalMap based on updated map and previous values
     * @param oldest the goal case
     */
    void printInternalMap(Pos oldest);

    std::deque<std::deque<Case>> m_internalMap;
    Pos m_basePos;
    Pos m_currentPos;

    /**
     * @return the score when the vacuum discover a new case
     */
    float getScoreDiscoverCase();
};

#endif
