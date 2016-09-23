#include "Pos.h"

std::ostream& operator<<(std::ostream& output, const Pos& position) {
    output << "(" << position.x << ", " << position.y << ")";
    return output;
}
bool operator==(const Pos& p1, const Pos& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}
