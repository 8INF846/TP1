#include "StateStrategyStates.h"
#include <iostream>

Action GoToLatestVisitedCaseState::execute(StateStrategy* str) {
    std::cout << "[STATE] Go to latest visited case" << std::endl;
    // Si batterie faible, rejoindre la base
    if(str->smartBatteryLevel() <= 2 * MOVEMENT_BATTERY_COST) {
        str->setState(GoToBaseAndChargeBatteryState::getInstance());
        return Action();
    }
    // Si des bijoux sur la case courante, les ramasser
    if(str->jewelry() > 0) {
        str->setState(GatherJewelryState::getInstance());
        return Action();
    }
    // Si de la saleté sur la case courante, l'aspirer
    if(str->dirtLevel() >= MIN_DIRT_LEVEL_TO_SUCK) {
        str->setState(SuckDirtState::getInstance());
        return Action();
    }
    ActionType direction = str->actionTypeToLatestVisitedCase();
    str->go(direction);
    return Action(direction);
}

Action GoToBaseAndChargeBatteryState::execute(StateStrategy* str) {
    std::cout << "[STATE] Go to base and charge battery" << std::endl;
    // Si la batterie est pleine
    if(str->batteryFull()) {
        str->setState(GoToLatestVisitedCaseState::getInstance());
        return Action();
    }
    if(str->isOnBase()) {
        return Action(Iddle, (100 - str->batteryLevel()) * CHARGE_TIME_COST);
    }
    ActionType direction = str->actionTypeToBase();
    str->go(direction);
    return Action(direction);
}

Action GatherJewelryState::execute(StateStrategy* str) {
    std::cout << "[STATE] Gather jewelry" << std::endl;
    if(str->jewelry() == 0) {
        str->setState(GoToLatestVisitedCaseState::getInstance());
        return Action();
    }
    if(str->smartBatteryLevel() < GATHER_BATTERY_COST) {
        str->setState(GoToBaseAndChargeBatteryState::getInstance());
        return Action();
    }
    return Action(Gather);
}

Action SuckDirtState::execute(StateStrategy* str) {
    std::cout << "[STATE] Suck dirt" << std::endl;
    if(str->smartBatteryLevel() <= 0) {
        str->setState(GoToBaseAndChargeBatteryState::getInstance());
        return Action();
    }
    if(str->jewelry() > 0) {
        str->setState(GatherJewelryState::getInstance());
        return Action();
    }
    if(str->dirtLevel() < MIN_DIRT_LEVEL_TO_SUCK) {
        str->setState(GoToLatestVisitedCaseState::getInstance());
        return Action();
    }
    float timer = std::min(str->smartBatteryLevel() / SUCK_BATTERY_COST,
            str->dirtLevel()) * SUCK_TIME_COST;
    return Action(Suck, timer);
}
