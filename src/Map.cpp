#include "Map.h"

#include <iostream>
#include <chrono>
#include <random>
#include <exception>

Map::Map(unsigned int width, unsigned int height) : m_cases(height, std::vector<Case>(width))
{}

bool Map::isFloor(Pos p) const {
    if(p.x < 0 || p.x >= int(width()) || p.y < 0 || p.y >= int(height())){
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
    if(p.x < 0 || p.x >= int(width()) || p.y < 0 || p.y >= int(height())){
        throw std::string("Position out of map cannot be changed");
    }
    m_cases[p.y][p.x].isFloor = isFloor;
}

void Map::addJewel(Pos p) {
    if(p.x < 0 || p.x >= int(width()) || p.y < 0 || p.y >= int(height())){
        throw std::string("Position out of map cannot be changed");
    }
    m_cases[p.y][p.x].jewelry++;
}

void Map::gatherJewelry(Pos p) {
    if(p.x < 0 || p.x >= int(width()) || p.y < 0 || p.y >= int(height())){
        throw std::string("Position out of map cannot be changed");
    }
    m_cases[p.y][p.x].jewelry--;
}

void Map::addDirt(Pos p, double delta) {
    if(p.x < 0 || p.x >= int(width()) || p.y < 0 || p.y >= int(height())){
        throw std::string("Position out of map cannot be changed");
    }
    m_cases[p.y][p.x].dirtLevel += delta;
    if(m_cases[p.y][p.x].dirtLevel > 1) {
        m_cases[p.y][p.x].dirtLevel = 1;
    }
}

void Map::suckDirt(Pos p) {
    if(p.x < 0 || p.x >= int(width()) || p.y < 0 || p.y >= int(height())){
        throw std::string("Position out of map cannot be changed");
    }
    m_cases[p.y][p.x].dirtLevel = 0;
}

unsigned int Map::width() const {
    return m_cases.at(0).size();
}

unsigned int Map::height() const {
    return m_cases.size();
}

void Map::update(double delta) {
    std::random_device rd;
    std::default_random_engine mt(rd());
    std::uniform_int_distribution<int> random_event(0, 5);
    std::uniform_int_distribution<int> random_ms(10, 1000);
    //Modify map
    std::uniform_int_distribution<int> random_height(0, m_cases.size()-1);
    auto h = random_height(mt);
    std::uniform_int_distribution<int> random_width(0, m_cases[h].size()-1);
    auto w = random_width(mt);
    Pos currentPos;
    currentPos.x = w;
    currentPos.y = h;
    if(random_event(mt) == 1) {
        std::cout << "Add jewel at pos (" << h << ";" << w << ")" << std::endl;
        addJewel(currentPos);
    } else {
        if(m_cases[h][w].dirtLevel < 1.0) {
            std::cout << "Add dirt at pos (" << h << ";" << w << ")" << std::endl;
            addDirt(currentPos, 0.1);
        }
    }
    //Sleep
    auto delay = std::chrono::milliseconds(random_ms(mt));
    std::cout << "sleep " << std::chrono::duration_cast<std::chrono::milliseconds>(delay).count() << " ms" << std::endl;
    std::this_thread::sleep_for(delay);
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