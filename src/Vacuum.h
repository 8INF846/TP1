#ifndef VACUUM_H
#define VACUUM_H

#include <memory>
#include "Pos.h"

class Vacuum {
    std::shared_ptr<Strategy> _strategy;
    float _score;
    Pos _currentPos;
    float _energy;
    Pos _pBase;
    Map& _map;
    MapProxy _proxy;
    Action _currentAction;
    void run();
    bool _shouldStop;
public
    Vacuum(std::shared_ptr<Strategy> strategy, Map& map, Pos pBase);:
    //Vacuum(std::shared_ptr<Strategy> strategy, Map& map, int iBase, int jBase);
    void start();
}

#endif
