#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <exception>
#include <regex>
#include <algorithm>

#include "Loader.h"
#include "Pos.h"
#include "SuckWithLevelStrategy.h"

Map Loader::loadMap(const std::string& filename) {
    //Read the map file
    std::vector<std::vector<bool>> floor;
    size_t width = 0;
    std::ifstream file(filename, std::ios::app);
    if(!file.is_open()) {
        throw std::string("Can't open file");
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

    //Create the map
    Map map(width, height);
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

Vacuum Loader::loadVacuum(const std::string& filename, Map& map) {
    std::ifstream file(filename);
    if(!file.is_open()) {
        throw std::string("Can't open file");
    }
    std::string line;
    std::string name;

    std::unique_ptr<Strategy> strategy;
    Pos basePos;
    basePos.x = -1;
    basePos.y = -1;
    while (std::getline(file, line))
    {
        const std::string s = line;
        //Parse the file
        if(s.size() > 0 && s[0] != '#') {
            const std::regex parameter("(.+)=(.*)");
            std::smatch match;
            std::smatch micmatch;

            if (std::regex_search(s.begin(), s.end(), match, parameter))
            {
                std::string param = std::string(match[1]);
                std::transform(param.begin(), param.end(), param.begin(), ::tolower);
                std::string value = std::string(match[2]);
                std::transform(value.begin(), value.end(), value.begin(), ::tolower);
                if(param == "strategy") {
                    //Note we just have one strategy for now
                    if(value != "suckwithlevel") {
                        throw std::string("Can't choose unknown strategy: " + value);
                    } else {
                        strategy = std::make_unique<SuckWithLevelStrategy>();
                    }
                } else if (param == "base") {
                    //The position of the vacuum at t=0
                    std::string pos = value;
                    std::string current;
                    for(size_t c = 0; c < pos.size(); ++c) {
                        if(pos[c] == '|' || (c+1 == pos.size())) {
                            if(c+1 == pos.size()) current += pos[c];
                            basePos.y = std::stoi(current);
                        }
                        else if(pos[c] == ';') {
                            basePos.x = std::stoi(current);
                            current = "";
                        } else {
                            current += pos[c];
                        }
                    }
                }
            }
        }
    }

    try {
        if(!map.isFloor(basePos)) {
            throw std::string("Can't set basePos for Vacuum");
        }
    } catch(const std::string & e) {
        throw std::string("Can't set basePos for Vacuum");
    }

    return Vacuum(strategy, basePos, map);
}
