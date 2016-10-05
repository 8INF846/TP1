#ifndef VACUUM_H
#define VACUUM_H

#include <memory>
#include <vector>

#include "Map.h"
#include "Startable.h"
#include "Pos.h"
#include "Strategy.h"
#include "Action.h"
#include "Sensors.h"


class Vacuum : public Startable {
public:
    /* Constructors */
    Vacuum() {}
    Vacuum(std::unique_ptr<Strategy>& strategy, Pos basePosition, Map& map);

    /* Methods */
    bool isBusy();
    bool batteryIsEmpty();
    Pos position() const;
    Pos basePosition() const;

private:
    /* Methods */
    void update(double delta);
    Sensors observe() const;
    Action findNextAction(const Sensors& sensors);
    void execute(Action action);

    /* Attributes */
    double m_dBattery;
    Pos m_position;
    Pos m_basePosition;
    std::unique_ptr<Strategy> m_strategy;
    Action m_currentAction;
    Map* m_map;
};

#endif
