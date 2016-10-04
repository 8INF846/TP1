#ifndef MAP_H
#define MAP_H

#include "Startable.h"
#include "Pos.h"
#include "Case.h"
#include <math.h>
#include <vector>

class Map : public Startable {
public:

    /**
     * @param p the position to test
     * @return if (p.x;p.y) is a floor
     */
    virtual bool isFloor(Pos p) const = 0;

    /**
     * @param p the position to test
     * @return dirtLevel at Pos p
     */
    virtual float dirtLevel(Pos p) const = 0;

    /**
     * @param p the position to test
     * @return jewelry at pos p
     */
    virtual int jewelry(Pos p) const = 0;

    /**
     * @return the width of the map
     */
    virtual unsigned int width() const = 0;

    /**
     * @return the height of the map
     */
    virtual unsigned int height() const = 0;

    /**
     * Change value of isFoor for case at pos p
     * @param p
     * @param isFloor
     */
    virtual void setIsFloor(Pos p, bool isFloor) = 0;

    /**
     * Add one jewel at pos p
     * @param p
     */
    virtual void addJewel(Pos p) = 0;

    /**
     * Remove jewel(s) at pos p
     * @param p
     */
    virtual void gatherJewelry(Pos p) = 0;

    /**
     * Add delta dirtLevel at pos p
     * @param p
     * @param delta
     */
    virtual void addDirt(Pos p, double delta) = 0;

    /**
     * Remove dirl at pos p
     * @param p
     */
    virtual void suckDirt(Pos p) = 0;

    /**
     * Update map
     */
    virtual void update(double delta = 0) = 0;
};

#endif
