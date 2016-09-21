#ifndef VACUUM_H
#define VACUUM_H

#include <memory>
#include <thread>

#include "Pos.h"
#include "Map.h"

class Vacuum {
    //std::shared_ptr<Strategy> _strategy;
    //MapProxy _proxy;
    //Action _currentAction;
public:
    Vacuum(Map& map, const Pos pBase);
    //Vacuum(std::shared_ptr<Strategy> strategy, Map& map, int iBase, int jBase);
    std::thread start();
    void run();
    void stop();

private:

    bool _shouldStop;
    float _score;
    Pos _currentPos;
    float _energy;
    const Pos _pBase;
    Map& _map;
};

#endif
