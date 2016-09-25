#include <vector>
#include <fstream>
#include <string>

#include "Loader.h"
#include "Pos.h"

static MapReal Loader::loadMap(const std::string& filemane) {
    // Lire le fichier
    std::vector<std::vector<bool>> floor;
    unsigned int width;
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
    MapReal map(height, width);
    for(unsigned int i = 0; i < height; i++) {
        for(unsigned int j = 0; j < width; j++) {
            if(floor[i].size() > j) {
                Pos position;
                position.x = i;
                position.y = j;
                map.setIsFloor(position, floor[i][j]);
            }
        }
    }
    return map;
}

static Vacuum Loader::loadVacuum(const std::string& filename) {
    // Lire le fichier
    std::ifstream file(filename, std::ios::app);
    if(!file.is_open()) {
        throw "Can't open file";
    }
    std::string line;

    // Lire la stratégie
    if(!std::getline(file, line)) {
        throw "Missing strategy name";
    }
    std::unique_ptr<Strategy> strategy;
    if(line.compare("SuckWithLevel")) {
        strategy = std::make_unique<SuckWithLevelStrategy>();
    }
    else {
        throw "Unknown strategy name";
    }

    // Lire la position
    if(!std::getline(file, line)) {
        throw "Missing base position";
    }
    Pos basePosition;
    std::size_t space;
    basePosition.x = std::stoi(line, &space);
    basePosition.y = std::stoi(line.substr(space));

    return Vacuum(strategy, basePosition);
}
