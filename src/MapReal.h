#ifndef MAP_REAL_H
#define MAP_REAL_H

#include <ostream>
#include <vector>
#include <thread>

#include "Map.h"
#include "Case.h"

class MapReal : public Map {
public:
    MapReal();
    MapReal(unsigned int width, unsigned int height);
    bool isFloor(Pos p) const;
    float dirtLevel(Pos p) const;
    int jewelry(Pos p) const;
    void setIsFloor(Pos p, bool isFloor);
    void setDirtLevel(Pos p, float dirtLevel);
    void setJewelry(Pos p, int jewelry);
    unsigned int width() const;
    unsigned int height() const;
    friend std::ostream& operator<<(std::ostream& output, const MapReal& mapReal);

    void update();
    std::thread start();

private:
    std::vector<std::vector<Case>> _cases;
};

#endif
