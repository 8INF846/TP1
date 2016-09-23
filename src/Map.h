#ifndef MAP_H
#define MAP_H

#include "Startable.h"
#include "Pos.h"
#include <math.h>

class Map : public Startable {
public:
    /* Constructors */
    MapReal(unsigned int width, unsigned int height);

    /* Methods */
    bool isFloor(Pos p) const = 0;
    float dirtLevel(Pos p) const = 0;
    int jewelry(Pos p) const = 0;
    unsigned int width() const;
    unsigned int height() const;
    void gatherJewelry(Pos p);
    void suckDirt(Pos p, double delta);
    friend std::ostream& operator<<(std::ostream& output, const Map& map);

private:
    /* Methods */
    void update();

    std::vector<std::vector<Case>> m_cases;
};

#endif
