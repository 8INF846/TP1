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
    void addJewel(Pos p);
    void gatherJewelry(Pos p);
    void addDirt(Pos p, double delta);
    void suckDirt(Pos p, double delta);
    friend std::ostream& operator<<(std::ostream& output, const MapReal& mapReal);

    void update(double delta);

private:
    std::vector<std::vector<Case>> m_cases;
};

#endif
