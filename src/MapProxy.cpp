#include "MapProxy.h"

MapProxy::MapProxy(const Vacuum& vacuum, const Map& map) : vacuum(vacuum), map(map) {}

bool MapProxy::isFloor(Pos p) {
    return map.isFloor(p);
}

float MapProxy::dirtLevel(Pos p) {
    if(!vacuum.canInspect(p)) {
        throw "Operation impossible";
    }
    return map.dirtLevel(p);
}

int MapProxy::jewelry(Pos p) {
    if(!vacuum.canInspect(p)) {
        throw "Operation impossible";
    }
    return map.jewelry(p);
}
