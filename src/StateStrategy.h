#ifndef STATE_STRATEGY_H
#define STATE_STRATEGY_H

#include "Strategy.h"
#include "Action.h"
#include "Pos.h"
#include "Case.h"
#include "State.h"
#include "StrCase.h"

#include <deque>
#include <vector>

class StateStrategy : public Strategy {
public:
    StateStrategy();
    Action findNextAction(const Sensors& sensors);

    bool batteryFull() const;
    double batteryLevel() const;
    double smartBatteryLevel();

    int jewelry() const;
    double dirtLevel() const;
    bool isOnBase() const;

    ActionType actionTypeToBase();
    ActionType actionTypeToLatestVisitedCase();
    Pos positionOfLatestVisitedCase() const;
    std::vector<ActionType> pathTo(const Pos& target);
    double distanceTo(const Pos& position) const;

    void setState(State* state);
    void go(ActionType direction);
    void updateLastTimeVisited();

private:
    void updateInternalMap(const Sensors& sensors);
    void displayInternalMap();

    std::vector<ActionType> constructPath(Pos target, Pos source);

    std::deque<std::deque<StrCase>> m_map;
    Pos m_position;
    Pos m_basePosition;
    double m_battery;
    State* m_state;
};

#endif
