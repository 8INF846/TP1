#include "MapProxy.h"

MapProxy::MapProxy(const Vacuum& vacuum, const Map& map) : vacuum(vacuum), map(map) {}

bool MapProxy::isFloor(int i, int j) {
    return map.isFloor(i, j);
}

float MapProxy::dirtLevel(int i, int j) {
    if(!vacuum.canInspect(i, j)) {
        throw "Operation impossible";
    }
    return map.dirtLevel(i, j);
}

int MapProxy::jewelry(int i, int j) {
    if(!vacuum.canInspect(i, j)) {
        throw "Operation impossible";
    }
    return map.jewelry(i, j);
}
