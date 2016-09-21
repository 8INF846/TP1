#include "MapReal.h"

#include <iostream>
#include <chrono>
#include <random>

MapReal::MapReal(unsigned int width, unsigned int height) : _cases(width, std::vector<Case>(height)) {}

bool MapReal::isFloor(Pos p) const {
    return _cases[p.y][p.x].isFloor;
}

float MapReal::dirtLevel(Pos p) const {
    return _cases[p.y][p.x].dirtLevel;
}

int MapReal::jewelry(Pos p) const {
    return _cases[p.y][p.x].jewelry;
}

void MapReal::setIsFloor(Pos p, bool isFloor) {
    _cases[p.y][p.x].isFloor = isFloor;
}

void MapReal::setDirtLevel(Pos p, float dirtLevel) {
    _cases[p.y][p.x].dirtLevel = dirtLevel;
}

void MapReal::setJewelry(Pos p, int jewelry) {
    _cases[p.y][p.x].jewelry = jewelry;
}

unsigned int MapReal::width() {
    return _cases.at(0).size();
}

unsigned int MapReal::height() {
    return _cases.size();
}

void MapReal::update() {
    std::random_device rd;
    std::default_random_engine mt(rd());
    std::uniform_int_distribution<int> random_event(0, 5);
    std::uniform_int_distribution<int> random_ms(10, 1000);
    while(true) {
        //Modify map
        std::uniform_int_distribution<int> random_height(0, _cases.size()-1);
        auto h = random_height(mt);
        std::uniform_int_distribution<int> random_width(0, _cases[h].size()-1);
        auto w = random_width(mt);
        Pos currentPos;
        currentPos.x = w;
        currentPos.y = h;
        if(random_event(mt) == 1) {
            std::cout << "Add jewel at pos (" << h << ";" << w << ")" << std::endl;
            setDirtLevel(currentPos, _cases[h][w].jewelry + 1);
        } else {
            if(_cases[h][w].dirtLevel < 1.0) {
                std::cout << "Add dirt at pos (" << h << ";" << w << ")" << std::endl;
                setDirtLevel(currentPos, _cases[h][w].dirtLevel + 0.1);
            }
        }
        //Sleep
        auto delay = std::chrono::milliseconds(random_ms(mt));
        std::cout << "sleep " << std::chrono::duration_cast<std::chrono::milliseconds>(delay).count() << " ms" << std::endl;
        std::this_thread::sleep_for(delay);
    }
}

std::thread MapReal::start() {
    return std::thread(&MapReal::update, this);
}

std::ostream& operator<<(std::ostream& output, const MapReal& mapReal) {
    if(mapReal._cases.size() > 0) {
        output << "┏";
        for(unsigned int i = 0; i < mapReal._cases[0].size(); i++) {
            output << "━";
        }
        output << "┓" << std::endl;
        for(unsigned int i = 0; i < mapReal._cases.size(); i++) {
            output << "┃";
            for(unsigned int j = 0; j < mapReal._cases[i].size(); j++) {
                output << (mapReal._cases[i][j].isFloor ? " " : "▓");
            }
            output << "┃" << std::endl;
        }
        output << "┗";
        for(unsigned int i = 0; i < mapReal._cases[0].size(); i++) {
            output << "━";
        }
        output << "┛" << std::endl;
    }
    else {
        output << "" << std::endl << "";
    }
    return output;
}
