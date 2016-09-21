#include "SuckWithLevelStrategy.h"

#include <iostream>

SuckWithLevelStrategy::SuckWithLevelStrategy(MapReal& map, const Pos pBase) : Strategy(map, pBase) {
    initializeInternalMap();
    _suckLevel = 0.2;
}

void SuckWithLevelStrategy::initializeInternalMap() {
    for(unsigned int h = 0; h <= _map.height(); ++h) {
        _internalMap.push_back(std::vector<Case>());
        for(unsigned int w = 0; w <= _map.width(); ++w) {
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
                //TODO update the probability of dirtLevelProb & jewel, or replace lastVisit by lastSucked?
                //Update case state
                Case currentCase;
                currentCase.dirtLevel = _map.dirtLevel(pCase);
                currentCase.jewelry = _map.jewelry(pCase);
                currentCase.lastVisit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                _internalMap[h][w] = currentCase;
                std::cout << "[Vacuum] Update case at (" <<  w << ";" << h << ")" << std::endl;
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
    Action chosenAction = pickAction();
    //Execute Action
    //TODO when execute = real score + time management and protect map to multithread changes
    if(chosenAction == IddleAction) {
        std::cout << "[Vacuum] IDDLE(" << _currentPos.x << ";" << _currentPos.y << ")" << std::endl;
        if(_pBase.x == _currentPos.x && _pBase.y == _currentPos.y) {
            std::cout << "[Vacuum] is fully charged" << std::endl;
            _energy = 100;
        }
    }
    if(chosenAction == SuckDirtAction) {
        std::cout << "[Vacuum] SUCKDIRT(" << _currentPos.x << ";" << _currentPos.y << "); "
        << "D:" << _map.dirtLevel(_currentPos)
        << "J:" << _map.jewelry(_currentPos)
        << std::endl;
        _map.setDirtLevel(_currentPos, 0);
        _map.setJewelry(_currentPos, 0);
    }
    if(chosenAction == SuckJewelAction) {
        std::cout << "[Vacuum] SUCKJEWEL(" << _currentPos.x << ";" << _currentPos.y << "); "
        << "D:" << _map.dirtLevel(_currentPos)
        << "J:" << _map.jewelry(_currentPos)
        << std::endl;
        _map.setJewelry(_currentPos, 0);
    }
    if(chosenAction == MoveUpAction) {
        _energy -= 1.0;
        _currentPos.y -= 1;
        std::cout << "[Vacuum] MOVE(" << _currentPos.x << ";" << _currentPos.y << "); " << std::endl;
    }
    if(chosenAction == MoveDownAction) {
        _energy -= 1.0;
        _currentPos.y += 1;
        std::cout << "[Vacuum] MOVE(" << _currentPos.x << ";" << _currentPos.y << "); " << std::endl;
    }
    if(chosenAction == MoveLeftAction) {
        _energy -= 1.0;
        _currentPos.x -= 1;
        std::cout << "[Vacuum] MOVE(" << _currentPos.x << ";" << _currentPos.y << "); " << std::endl;
    }
    if(chosenAction == MoveRightAction) {
        _energy -= 1.0;
        _currentPos.x += 1;
        std::cout << "[Vacuum] MOVE(" << _currentPos.x << ";" << _currentPos.y << "); " << std::endl;
    }
}

float SuckWithLevelStrategy::getScoreMove(Pos p) {
    if(!_map.isFloor(p)) throw "Can't get score of a non floor case";

    auto score = -1.;
    if(_internalMap[p.y][p.x].dirtLevel == UNKNOWN_STATUS) {
        score = 1000;
    } else {
        //if it's the origin we can iddle next (it's good if we are low energy)
        if(_pBase.x == p.x && _pBase.y == p.y) {
            score += (100-_energy)/10;
        }
        auto lastVisit = _internalMap[p.y][p.x].lastVisit;
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto timeDiff = now - lastVisit;
        auto guessDirtLevel = (_internalMap[p.y][p.x].dirtLevel + timeDiff*_dirtLevelProb);
        if(guessDirtLevel > 1.0) guessDirtLevel = 1.0;
        score += guessDirtLevel * 100;
        auto guessJewelry = (_internalMap[p.y][p.x].jewelry + timeDiff*_jewelLevelProb);
        if(guessJewelry > 1.0) guessJewelry = 1.0;
        score += guessJewelry * 150;
    }
    return score;
}

Action SuckWithLevelStrategy::pickAction() {
    //Evaluate score if do action
    //TODO change score evaluation with tests ;)
    auto chosenAction = IddleAction;
    auto x = _currentPos.x;
    auto y = _currentPos.y;
    //1. Iddle
    auto scoreIddle = .0;
    if(_pBase.x == x && _pBase.y == y) {
        scoreIddle = 100-_energy; //if the energy is low it's better to stay
    }
    auto currentScore = scoreIddle;
    std::cout << "iddle:" << scoreIddle << std::endl;
    //2. SuckDirt
    auto scoreSuckDirt = -1.0; //We loose 1% of energy
    scoreSuckDirt += 10*_internalMap[y][x].dirtLevel;
    scoreSuckDirt -= 100*_internalMap[y][x].jewelry;
    if(scoreSuckDirt > currentScore) chosenAction = SuckDirtAction;
    std::cout << "dirt:" << scoreSuckDirt << std::endl;
    //3. SuckJewel
    auto scoreSuckJewel = -1.0; //We loose 1% of energy
    scoreSuckJewel += 10*_internalMap[y][x].jewelry;
    if(scoreSuckJewel > currentScore) chosenAction = SuckJewelAction;
    std::cout << "jewel:" << scoreSuckJewel << std::endl;
    //4. TODO move (prévision 1 case)
    //UP
    Pos up;
    up.x = x;
    up.y = y-1;
    if(_map.isFloor(up)) {
        auto scoreUp = getScoreMove(up);
        if(scoreUp > currentScore) chosenAction = MoveUpAction;
        std::cout << "Up:" << scoreUp << std::endl;
    }
    //DOWN
    Pos down;
    down.x = x;
    down.y = y+1;
    if(_map.isFloor(down)) {
        auto scoreDown = getScoreMove(down);
        if(scoreDown > currentScore) chosenAction = MoveDownAction;
        std::cout << "Down:" << scoreDown << std::endl;
    }
    //LEFT
    Pos left;
    left.x = x-1;
    left.y = y;
    if(_map.isFloor(left)) {
        auto scoreLeft = getScoreMove(left);
        if(scoreLeft > currentScore) chosenAction = MoveLeftAction;
        std::cout << "Left:" << scoreLeft << std::endl;
    }
    //RIGHT
    Pos right;
    right.x = x+1;
    right.y = y;
    if(_map.isFloor(right)) {
        auto scoreRight = getScoreMove(right);
        if(scoreRight > currentScore) chosenAction = MoveRightAction;
        std::cout << "Right:" << scoreRight << std::endl;
    }

    return chosenAction;
}
