#ifndef SUCKWITHLEVELSTRATEGY_H
#define SUCKWITHLEVELSTRATEGY_H

#include "Strategy.h"
#include "Action.h"
#include "Pos.h"
#include "StrCase.h"

#include <deque>
#include <random>

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
     * Update internalPosition
     */
    void simulateInternalAction(ActionType action);

    /**
     * Print m_internalMap based on updated map and previous values
     * @param oldest the goal case
     */
    void printInternalMap(Pos oldest);

    std::deque<std::deque<StrCase>> m_internalMap;
    Pos m_basePos;
    Pos m_currentPos;

    /**
     * @return the score when the vacuum discover a new case
     */
    float getScoreDiscoverCase();

    float m_fmaxScore;

    /**
     * Change m_fmaxScore if score is >=
     * @return if m_fmaxScore was changed
     */
    bool changeFMaxScore(const float score);

    std::random_device m_rd;
    std::mt19937 m_mt;
    std::uniform_int_distribution<int> m_dist;
};

#endif
