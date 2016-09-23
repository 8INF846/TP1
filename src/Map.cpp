#include "Map.h"

#include <iostream>
#include <chrono>
#include <random>

Map::Map(unsigned int width, unsigned int height) :
        m_cases(width, std::vector<Case>(height)) {}

bool Map::isFloor(Pos p) const {
    if(p.x < 0 || p.x > int(width()) || p.y < 0 || p.y > int(height())){
        return false;
    }
    return m_cases[p.y][p.x].isFloor;
}

float Map::dirtLevel(Pos p) const {
    return m_cases[p.y][p.x].dirtLevel;
}

int Map::jewelry(Pos p) const {
    return m_cases[p.y][p.x].jewelry;
}

void Map::setIsFloor(Pos p, bool isFloor) {
    if(p.x < 0 || p.x > int(width()) || p.y < 0 || p.y > int(height())){
        throw "Position out of map cannot be changed";
    }
    m_cases[p.y][p.x].isFloor = isFloor;
}

void Map::gatherJewelry(Pos p) {
    if(p.x < 0 || p.x > int(width()) || p.y < 0 || p.y > int(height())){
        throw "Position out of map cannot be changed";
    }
    m_cases[p.y][p.x].jewelry--;
}

void Map::suckDirt(Pos p, double delta) {
    if(p.x < 0 || p.x > int(width()) || p.y < 0 || p.y > int(height())){
        throw "Position out of map cannot be changed";
    }
    m_cases[p.y][p.x].dirtlevel -= delta;
    if(m_cases[p.y][p.x].dirtLevel < 0) {
        m_cases[p.y][p.x].dirtLevel = 0;
    }
}

unsigned int Map::width() const {
    return m_cases.at(0).size() - 1;
}

unsigned int Map::height() const {
    return m_cases.size() - 1;
}

void Map::update(double delta) {
    // TODO update de la map pendant delta secondes
}

std::ostream& operator<<(std::ostream& output, const Map& map) {
    if(map.m_cases.size() > 0) {
        output << "┏";
        for(unsigned int i = 0; i < map.m_cases[0].size(); i++) {
            output << "━";
        }
        output << "┓" << std::endl;
        for(unsigned int i = 0; i < map.m_cases.size(); i++) {
            output << "┃";
            for(unsigned int j = 0; j < map.m_cases[i].size(); j++) {
                output << (map.m_cases[i][j].isFloor ? " " : "▓");
            }
            output << "┃" << std::endl;
        }
        output << "┗";
        for(unsigned int i = 0; i < map.m_cases[0].size(); i++) {
            output << "━";
        }
        output << "┛" << std::endl;
    }
    else {
        output << "" << std::endl << "";
    }
    return output;
}
