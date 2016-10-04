#ifndef MAP_H
#define MAP_H

#include <ostream>
#include <vector>
#include <thread>

#include "Case.h"
#include "Pos.h"
#include "Startable.h"

class Map : public Startable {
public:
    /* CONSTRUCTORS */
    Map() {};
    Map(unsigned int width, unsigned int height);

    /* ACCESSORS */
    /**
     * @param p the position to test
     * @return wether there is floor at given position
     */
    bool isFloor(Pos p) const;

    /**
     * @param p the position to test
     * @return the dirt level at given position
     */
    float dirtLevel(Pos p) const;

    /**
     * @param p the position to test
     * @return the jewelry count at given position
     */
    int jewelry(Pos p) const;

    /**
     * @return the width of the map
     */
    unsigned int width() const;

    /**
     * @return the height of the map
     */
    unsigned int height() const;

    /* MUTATORS */
    /**
     * Set wether or not there is floor at given position.
     * @param p the position to set
     * @param isFloor wether or not there is floor at given position
     */
    void setIsFloor(Pos p, bool isFloor);

    /**
     * Set dirt level at given position.
     * @param p the position to set
     * @param dirtLevel the amout of dirt at given position
     */
    void setDirtLevel(Pos p, float dirtLevel);

    /**
     * Set jewelry count at given position.
     * @param p the position to set
     * @param jewelry the number of jewlery at given position
     */
    void setJewelry(Pos p, int jewelry);

    /**
     * Add one jewelry at given position.
     * @param p the position to update
     */
    void addJewel(Pos p);

    /**
     * Remove one jewelry at given position.
     * @param p the position to update
     */
    void gatherJewelry(Pos p);

    /**
     * Add dirt at given position (may be negative).
     * @param p the position to update
     * @param delta the amout of position to add/remove.
     */
    void addDirt(Pos p, double delta);

    /**
     * Remove all dirt at given position.
     * @param p the position to update
     */
    void suckDirt(Pos p);

    /**
     * Print a map
     */
    friend std::ostream& operator<<(std::ostream& output, const Map& map);

    /**
     * Must be implemented since it inherits from Startable.
     */
    void update(double delta);

private:
    std::vector<std::vector<Case>> m_cases;
};

#endif
