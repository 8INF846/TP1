#ifndef VACUUM_H
#define VACUUM_H

#include <memory>
#include <vector>

#include "Startable.h"
#include "Pos.h"
#include "Strategy.h"
#include "Action.h"
#include "Sensors.h"


class Vacuum : public Startable {
public:
    /* Constructors */
    Vacuum() {}
    Vacuum(std::unique_ptr<Strategy>& strategy, Pos basePosition, MapReal& map);

    /* Methods */
    bool isBusy();

private:
    /* Methods */
    void update(double delta);
    Sensors observe() const;
    void findNextAction(const Sensors& sensors);
    void executeCurrentAction(double delta);

    /* Attributes */
    double m_dBattery;
    Pos m_position;
    Pos m_basePosition;
    bool m_bShouldStop;
    std::unique_ptr<Strategy> m_strategy;
    Action m_currentAction;
    MapReal* m_map;
};

#endif
