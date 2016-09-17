#include "Environnement.h"
#include <iostream>
#include <vector>
#include <fstream>

Environnement::Environnement(const std::string& filename) {
    //TODO init _vacuum
    initializeMap(filename);
}

void Environnement::initializeMap(const std::string& filename) {
    std::ifstream inputMap(filename, std::ios::app);
    std::vector<std::vector<bool>> isFloorVec;
    if(inputMap.is_open())
    {
        std::string line;
        while (std::getline(inputMap, line))
        {
            std::vector<bool> lineVec;
            for(unsigned int c = 0; c < line.size(); ++c) {
                if(line.at(c) == '0') {
                    lineVec.push_back(false);
                }
                if(line.at(c) == '1') {
                    lineVec.push_back(true);
                }
            }
            isFloorVec.push_back(lineVec);
        }
    } else {
        throw "Can't open file";
    }

    unsigned int height = isFloorVec.size();
    unsigned int width = isFloorVec.at(0).size();
    _map = new MapReal(height, width);

    for(unsigned int h = 0; h < height; ++h) {
        for(unsigned int w = 0; w < width; ++w) {
            _map->setIsFloor(h, w, (isFloorVec[h].size() > w) ? isFloorVec[h][w] : false);
        }
    }

    std::cout << *_map;
}

void Environnement::start() {

}
