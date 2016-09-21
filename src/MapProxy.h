#ifndef MAP_PROXY_H
#define MAP_PROXY_H

#include "Vacuum.h"
#include "Map.h"

class MapProxy : public Map {
    const Vacuum& vacuum;
    const Map& map;
public:
    MapProxy(const Vacuum& vacuum, const Map& map);
};

#endif
