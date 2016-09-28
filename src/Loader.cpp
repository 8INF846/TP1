#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <exception>

#include "Loader.h"
#include "Pos.h"
#include "SuckWithLevelStrategy.h"

MapReal Loader::loadMap(const std::string& filename) {
    // Lire le fichier
    std::vector<std::vector<bool>> floor;
    size_t width = 0;
    std::ifstream file(filename, std::ios::app);
    if(!file.is_open()) {
        throw "Can't open file";
    }
    std::string line;
    while(std::getline(file, line)) {
        floor.emplace_back();
        width = std::max(width, line.size());
        for(unsigned int c = 0; c < line.size(); c++) {
            if(line.at(c) == '1') {
                floor.back().push_back(true);
            }
            else {
                floor.back().push_back(false);
            }
        }
    }
    unsigned int height = floor.size();

    // Créer la map
    MapReal map(width, height);
    for(unsigned int h = 0; h < height; h++) {
        for(unsigned int w = 0; w < width; w++) {
            if(floor[h].size() > w) {
                Pos position;
                position.x = w;
                position.y = h;
                try {
                    map.setIsFloor(position, floor[h][w]);
                } catch(const std::string & e) {
                    continue;
                }
            }
        }
    }
    return map;
}

Vacuum Loader::loadVacuum(const std::string& filename) {
    // Lire le fichier
    /*std::ifstream file(filename, std::ios::app);
    if(!file.is_open()) {
        throw "Can't open file";
    }
    std::string line;

    // Lire la stratégie
    if(!std::getline(file, line)) {
        throw "Missing strategy name";
    }*/
    std::unique_ptr<Strategy> strategy = std::make_unique<SuckWithLevelStrategy>();
    /*if(line.compare("SuckWithLevel")) {
        strategy = std::make_unique<SuckWithLevelStrategy>();
    }
    else {
        throw "Unknown strategy name";
    }

    // Lire la position
    if(!std::getline(file, line)) {
        throw "Missing base position";
    }*/
    Pos basePosition;
    //std::size_t space;
    basePosition.x = 0;//std::stoi(line, &space);
    basePosition.y = 0;//std::stoi(line.substr(space));

    return Vacuum(strategy, basePosition);
}
