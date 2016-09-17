#ifndef MAP_REAL_H
#define MAP_REAL_H

#include <ostream>
#include <vector>
#include "Map.h"
#include "Case.h"

class MapReal : public Map {
    std::vector<std::vector<Case>> cases;
public:
    MapReal(int width, int height);
    bool isFloor(int i, int j) const;
    float dirtLevel(int i, int j) const;
    int jewelry(int i, int j) const;
    void setIsFloor(int i, int j, bool isFloor);
    void setDirtLevel(int i, int j, float dirtLevel);
    void setJewelry(int i, int j, int jewelry);
    friend std::ostream& operator<<(std::ostream& output, const MapReal& mapReal);
};

#endif
