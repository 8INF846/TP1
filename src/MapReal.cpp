#include "MapReal.h"


MapReal::MapReal(int width, int height) : cases(width, std::vector<Case>(height)) {}

bool MapReal::isFloor(int i, int j) const {
    return cases[i][j].isFloor;
}

float MapReal::dirtLevel(int i, int j) const {
    return cases[i][j].dirtLevel;
}

int MapReal::jewelry(int i, int j) const {
    return cases[i][j].jewelry;
}

void MapReal::setIsFloor(int i, int j, bool isFloor) {
    cases[i][j].isFloor = isFloor;
}

void MapReal::setDirtLevel(int i, int j, float dirtLevel) {
    cases[i][j].dirtLevel = dirtLevel;
}

void MapReal::setJewelry(int i, int j, int jewelry) {
    cases[i][j].jewelry = jewelry;
}

std::ostream& operator<<(std::ostream& output, const MapReal& mapReal) {
    if(mapReal.cases.size() > 0) {
        output << "┏";
        for(int i = 0; i < mapReal.cases[0].size(); i++) {
            output << "━";
        }
        output << "┓" << std::endl;
        for(int i = 0; i < mapReal.cases.size(); i++) {
            output << "┃";
            for(int j = 0; j < mapReal.cases[i].size(); j++) {
                output << (mapReal.cases[i][j].isFloor ? " " : "▓");
            }
            output << "┃" << std::endl;
        }
        output << "┗";
        for(int i = 0; i < mapReal.cases[0].size(); i++) {
            output << "━";
        }
        output << "┛" << std::endl;
    }
    else {
        output << "" << std::endl << "";
    }
    return output;
}

