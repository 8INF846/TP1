#ifndef VACUUM_H
#define VACUUM_H

#include <memory>

class Vacuum {
    std::shared_ptr<Strategy> _strategy;
    float _score;
    int _i;
    int _j;
    float _energy;
    int _iBase;
    int _jBase;
    Map& _map;
    MapProxy _proxy;
    Action _currentAction;
    void run();
    bool _shouldStop;
public:
    Vacuum(std::shared_ptr<Strategy> strategy, Map& map, int iBase, int jBase);
    void start();
}

#endif
