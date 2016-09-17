#ifndef MAP_REAL_H
#define MAP_REAL_H

#include <ostream>
#include <vector>
#include <thread>

#include "Map.h"
#include "Case.h"

class MapReal : public Map {
public:
    MapReal(int width, int height);
    bool isFloor(int i, int j) const;
    float dirtLevel(int i, int j) const;
    int jewelry(int i, int j) const;
    void setIsFloor(int i, int j, bool isFloor);
    void setDirtLevel(int i, int j, float dirtLevel);
    void setJewelry(int i, int j, int jewelry);
    friend std::ostream& operator<<(std::ostream& output, const MapReal& mapReal);

    void update();
    std::thread run();

private:
    std::vector<std::vector<Case>> _cases;
};

#endif
