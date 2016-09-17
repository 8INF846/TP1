#ifndef MAP_H
#define MAP_H

class Map {
public:
    // GETTERS
    virtual bool isFloor(int i, int j) const = 0;
    virtual float dirtLevel(int i, int j) const = 0;
    virtual int jewelry(int i, int j) const = 0;
};

#endif
