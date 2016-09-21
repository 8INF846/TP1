#ifndef VACUUM_H
#define VACUUM_H

#include <memory>
#include <thread>
#include <vector>

#include "Pos.h"
#include "Case.h"
#include "Strategy.h"

class Vacuum {
    //Action _currentAction;
public:
    Vacuum(MapReal& map, const Pos pBase);
    //Vacuum(std::shared_ptr<Strategy> strategy, Map& map, int iBase, int jBase);
    std::thread start();
    void run();
    void stop();

private:
    bool _shouldStop;
    Strategy* _strategy; //Unique_ptr

    //TODO move in another object (Strategy)
    void updateInternalMap();
    void chooseAction();
};

#endif
