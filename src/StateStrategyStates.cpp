#include "StateStrategyStates.h"
#include <iostream>
#include "Console.h"

Action GoToLatestVisitedCaseState::execute(StateStrategy* str) {
    std::ostream& out = Console::out(2);
    out << "State : GoToLatestVisitedCase" << std::endl;
    std::ostream& out4 = Console::out(4);

    // Si batterie faible, rejoindre la base
    if(str->smartBatteryLevel() <= 2 * MOVEMENT_BATTERY_COST) {
        out4 << "<battery low>";
        str->setState(GoToBaseAndChargeBatteryState::getInstance());
        return Action();
    }
    // Si des bijoux sur la case courante, les ramasser
    if(str->jewelry() > 0) {
        out4 << "<jewelry>";
        str->setState(GatherJewelryState::getInstance());
        return Action();
    }
    // Si de la saleté sur la case courante, l'aspirer
    if(str->dirtLevel() >= MIN_DIRT_LEVEL_TO_SUCK) {
        out4 << "<dirt>";
        str->setState(SuckDirtState::getInstance());
        return Action();
    }
    else {
        str->updateLastTimeVisited();
    }
    out4 << "<move>";
    ActionType direction;
    try {
        direction = str->actionTypeToLatestVisitedCase();
    } catch (const std::string& e) {
        direction = Iddle;
    }

    str->go(direction);
    return Action(direction);
}

Action GoToBaseAndChargeBatteryState::execute(StateStrategy* str) {
    std::ostream& out = Console::out(2);
    out << "State : GoToBaseAndChargeBattery" << std::endl;

    // En passant, mettre à jour la dernière visite si la case n'est pas sale
    if(str->dirtLevel() < MIN_DIRT_LEVEL_TO_SUCK) {
        str->updateLastTimeVisited();
    }

    // Si la batterie est pleine
    if(str->batteryFull()) {
        str->setState(GoToLatestVisitedCaseState::getInstance());
        return Action();
    }
    if(str->isOnBase()) {
        // Plafoner le temps pour "voir le rechargement"
        auto missing = 100. - str->batteryLevel();
        return Action(Iddle, std::min(missing, 3.) * CHARGE_TIME_COST);
    }
    ActionType direction = str->actionTypeToBase();
    str->go(direction);
    return Action(direction);
}

Action GatherJewelryState::execute(StateStrategy* str) {
    std::ostream& out = Console::out(2);
    out << "State : GatherJewelry" << std::endl;

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
    std::ostream& out = Console::out(2);
    out << "State : SuckDirt" << std::endl;

    if(str->smartBatteryLevel() <= 0) {
        str->setState(GoToBaseAndChargeBatteryState::getInstance());
        return Action();
    }
    if(str->jewelry() > 0) {
        str->setState(GatherJewelryState::getInstance());
        return Action();
    }
    if(str->dirtLevel() < MIN_DIRT_LEVEL_TO_SUCK) {
        std::cout << str->dirtLevel() << std::endl;
        str->updateLastTimeVisited();
        str->setState(GoToLatestVisitedCaseState::getInstance());
        return Action();
    }
    double timer = std::min(str->smartBatteryLevel() / SUCK_BATTERY_COST,
            str->dirtLevel()) * SUCK_TIME_COST;
    // Plafonner le temps pour éviter d'aspirer un légo (ou un bijoux)
    if(timer < 2 * MAX_SUCKING_TIME) {
        timer = std::min(MAX_SUCKING_TIME, timer);
    }
    str->updateLastTimeVisited();
    return Action(Suck, timer);
}
