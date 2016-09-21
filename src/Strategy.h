#ifndef STRATEGY_H
#define STRATEGY_H

#include "MapReal.h"
#include "Pos.h"


class Strategy {
public:
    Strategy(MapReal& map, const Pos pBase) : _currentPos(pBase), _energy(100), _pBase(pBase), _map(map)
    {
        if(!map.isFloor(pBase)) {
            throw "Position de base invalide.";
        }
    }
    virtual void observeAndUpdate() = 0;
    virtual void pickAndExecAction() = 0;

protected:
    Pos _currentPos;
    bool canInspect(Pos p)  {
        return p.x == _currentPos.x && p.y == _currentPos.y;
    }
    
    float _energy;
    const Pos _pBase;
    MapReal& _map;
};

#endif
