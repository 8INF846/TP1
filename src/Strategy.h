#ifndef STRATEGY_H
#define STRATEGY_H

#include "MapReal.h"
#include "Pos.h"

class Strategy {
public:
    Strategy(MapReal& map, const Pos pBase) :
        _currentPos(pBase), _energy(100), _pBase(pBase), _map(map){}
    virtual void nextAction() = 0;

protected:
    Pos _currentPos;
    float _energy;
    const Pos _pBase;
    MapReal& _map;
};

#endif
