#ifndef STATE_STRATEGY_STATES_H
#define STATE_STRATEGY_STATES_H

#include <cmath>
#include "State.h"
#include "StateStrategy.h"
#include "Action.h"
#include "StrCase.h"

#define MOVEMENT_BATTERY_COST 1
#define MIN_DIRT_LEVEL_TO_SUCK 0.1
#define CHARGE_TIME_COST 0.5
#define GATHER_BATTERY_COST 1.4
#define SUCK_BATTERY_COST 1
#define SUCK_TIME_COST 2

class GoToBaseAndChargeBatteryState;
class GatherJewelryState;
class SuckDirtState;

/**
 * L'aspirateur essaye de rejoindre la case la plus anciennement visitée.
 */
class GoToLatestVisitedCaseState : public State {
public:
    static GoToLatestVisitedCaseState* getInstance() {
        static GoToLatestVisitedCaseState instance;
        return &instance;
    }
    Action execute(StateStrategy* str);
};

/**
 * L'aspirateur essaye de rejoindre la base.
 */
class GoToBaseAndChargeBatteryState : public State {
public:
    static GoToBaseAndChargeBatteryState* getInstance() {
        static GoToBaseAndChargeBatteryState instance;
        return &instance;
    }
    Action execute(StateStrategy* str);
};

/**
 * L'aspirateur récupère les bijoux à la position courante
 */
class GatherJewelryState : public State {
public:
    static GatherJewelryState* getInstance() {
        static GatherJewelryState instance;
        return &instance;
    }
    Action execute(StateStrategy* str);
};

/**
 * L'aspirateur aspire la saleté à la position courante
 */
class SuckDirtState : public State {
public:
    static SuckDirtState* getInstance() {
        static SuckDirtState instance;
        return &instance;
    }
    Action execute(StateStrategy* str);
};

#endif
