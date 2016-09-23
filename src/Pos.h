#ifndef POS_H
#define POS_H

#include <ostream>

struct Pos {
    int x;
    int y;
    friend bool operator==(const Pos& p1, const Pos& p2);
    friend std::ostream& operator<<(std::ostream& output, const Pos& position);
};

#endif
