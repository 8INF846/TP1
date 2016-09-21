#include "SuckWithLevelStrategy.h"

#include <iostream>

SuckWithLevelStrategy::SuckWithLevelStrategy(MapReal& map, const Pos pBase) : Strategy(map, pBase) {
    initializeInternalMap();
    _suckLevel = 0.2;
}

void SuckWithLevelStrategy::initializeInternalMap() {
    for(unsigned int h = 0; h < _map.height(); ++h) {
        _internalMap.push_back(std::vector<Case>());
        for(unsigned int w = 0; w < _map.width(); ++w) {
            Pos pCase;
            pCase.x = w;
            pCase.y = h;
            Case c;
            c.isFloor = _map.isFloor(pCase);
            c.dirtLevel = UNKNOWN_STATUS;
            c.jewelry = UNKNOWN_STATUS;
            _internalMap.at(h).push_back(c);
        }
    }
}

void SuckWithLevelStrategy::observeAndUpdate() {
    if(_internalMap.size() == 0) throw "[Vacuum] Internal Map is not initialized";

    auto height = _internalMap.size();
    auto width  = _internalMap.at(0).size();
    for(unsigned int h = 0; h < height; ++h) {
        for(unsigned int w = 0; w < width; ++w) {
            Pos pCase;
            pCase.x = w;
            pCase.y = h;
            if(canInspect(pCase)) {
                //TODO update the probability of dirtLevelProb & jewel
                //Update case state
                Case currentCase;
                currentCase.dirtLevel = _map.dirtLevel(pCase);
                currentCase.jewelry = _map.jewelry(pCase);
                currentCase.lastVisit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                _internalMap[h][w] = currentCase;
            std::cout << "[Vacuum] Update case at (" <<  w << ";" << h << ")" << _internalMap[h][w].dirtLevel << std::endl;
            } else {
                //Try to guess the state of the case now
                if(_internalMap[h][w].dirtLevel != UNKNOWN_STATUS) {
                    auto lastVisit = _internalMap[h][w].lastVisit;
                    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    auto timeDiff = now - lastVisit;
                    //Update case
                    _internalMap[h][w].dirtLevel += timeDiff*_dirtLevelProb;
                    _internalMap[h][w].jewelry += timeDiff*_jewelLevelProb;
                }
                //else we never go visit the case, so we can't guess the state
            }
        }
    }
}

void SuckWithLevelStrategy::pickAndExecAction() {
    //TODO Action object with time and score management
    if(_internalMap[_currentPos.y][_currentPos.x].dirtLevel >= _suckLevel) {
        //TODO protect Map for multithread changes
        std::cout << "[Vacuum] SUCK(" << _currentPos.x << ";" << _currentPos.y << ")" << std::endl;
        _map.setDirtLevel(_currentPos, 0);
    }
}
