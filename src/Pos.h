#ifndef POS_H
#define POS_H

#include <ostream>

// A struct that represents a Position
struct Pos {
    int x;
    int y;
    Pos();
    Pos(int x, int y);
    //Test if 2 pos are equals
    friend bool operator==(const Pos& p1, const Pos& p2);
    friend bool operator!=(const Pos& p1, const Pos& p2);
    //Print pos
    friend std::ostream& operator<<(std::ostream& output, const Pos& position);
};

#endif
