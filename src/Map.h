#ifndef MAP_H
#define MAP_H

#include "Startable.h"
#include "Pos.h"
#include "Case.h"
#include <math.h>
#include <vector>

class Map : public Startable {
public:
    /* Constructors */
    Map(unsigned int width, unsigned int height);

    /* Methods */
    virtual bool isFloor(Pos p) const = 0;
    virtual float dirtLevel(Pos p) const = 0;
    virtual int jewelry(Pos p) const = 0;
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
