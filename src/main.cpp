#include <iostream>
#include "MapReal.h"

int main() {
    MapReal map(3, 5);
    map.setIsFloor(0, 0, true);
    map.setIsFloor(0, 1, true);
    map.setIsFloor(0, 2, true);
    map.setIsFloor(1, 0, true);
    map.setIsFloor(1, 1, true);
    map.setIsFloor(1, 2, true);
    map.setIsFloor(1, 3, true);
    map.setIsFloor(1, 4, true);
    map.setIsFloor(2, 0, true);
    map.setIsFloor(2, 1, true);
    map.setIsFloor(2, 2, true);
    std::cout << map;
    return EXIT_SUCCESS;
}
