#include "SuckWithLevelStrategy.h"

#include <iostream>
#include <random>

SuckWithLevelStrategy::SuckWithLevelStrategy() : Strategy() {
    m_fenergy = 100.;
    //Add 1 case
    std::deque<Case> origin = std::deque<Case>(1);
    m_internalMap.push_back(origin);
}

Action SuckWithLevelStrategy::findNextAction(const Sensors& sensors)
{
    Action finalAction;
    updateInternalMap(sensors);


    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0,1);

    auto scoreIddle = getScoreIddle();
    auto maxScore = scoreIddle;

    if(sensors.north) {
        auto scoreMoveNorth = 0.0;
        if(m_internalMap[m_currentPos.y-1][m_currentPos.x].dirtLevel == UNKNOWN_STATUS) scoreMoveNorth += getScoreDiscoverCase();

        if(scoreMoveNorth > maxScore || (scoreMoveNorth == maxScore && dist(mt) == 0)) {
            finalAction.type = GoNorth;
            maxScore = scoreMoveNorth;
        }
    }

    if(sensors.south) {
        auto scoreMoveSouth = 0.0;
        if(m_internalMap[m_currentPos.y+1][m_currentPos.x].dirtLevel == UNKNOWN_STATUS) scoreMoveSouth += getScoreDiscoverCase();

        if(scoreMoveSouth > maxScore || (scoreMoveSouth == maxScore && dist(mt) == 0)) {
            finalAction.type = GoSouth;
            maxScore = scoreMoveSouth;
        }
    }

    if(sensors.east) {
        auto scoreMoveEast = 0.0;
        if(m_internalMap[m_currentPos.y][m_currentPos.x+1].dirtLevel == UNKNOWN_STATUS) scoreMoveEast += getScoreDiscoverCase();

        if(scoreMoveEast > maxScore || (scoreMoveEast == maxScore && dist(mt) == 0)) {
            finalAction.type = GoEast;
            maxScore = scoreMoveEast;
        }
    }

    if(sensors.west) {
        auto scoreMoveWest = 0.0;
        if(m_internalMap[m_currentPos.y][m_currentPos.x-1].dirtLevel == UNKNOWN_STATUS) scoreMoveWest += getScoreDiscoverCase();

        if(scoreMoveWest > maxScore || (scoreMoveWest == maxScore && dist(mt) == 0)) {
            finalAction.type = GoWest;
            maxScore = scoreMoveWest;
        }
    }

    //TODO Aller vers case éloignée (découvrir une nouvelle si il y en a,
    //ou aller vers la case la plus sale en passant ou non vers l'origine pour charger

    //Update internal state
    switch (finalAction.type)
    {
    case GoNorth:
        m_currentPos.y-=1;
        break;
    case GoWest:
        m_currentPos.x-=1;
        break;
    case GoEast:
        m_currentPos.x+=1;
        break;
    case GoSouth:
        m_currentPos.y+=1;
        break;
    default:
        break;
    }
    return finalAction;
}


float SuckWithLevelStrategy::getScoreDiscoverCase() {
    return 100.;
}

float SuckWithLevelStrategy::getScoreIddle() {
    return (m_currentPos.x == m_basePos.x)
    && (m_currentPos.y == m_basePos.y) ?
    100.-m_fenergy : 0;
}

void SuckWithLevelStrategy::updateInternalMap(const Sensors& sensors) {
    m_internalMap[m_currentPos.y][m_currentPos.x].isFloor = true;
    m_internalMap[m_currentPos.y][m_currentPos.x].dirtLevel = sensors.dirt;
    m_internalMap[m_currentPos.y][m_currentPos.x].jewelry = sensors.jewelry;

    if(m_currentPos.x == 0) {
        if(sensors.west) {
            //Add a col
            for(size_t h = 0; h < m_internalMap.size(); ++h) {
                Case c;
                c.isFloor = (int)h == m_currentPos.y;
                c.dirtLevel = UNKNOWN_STATUS;
                c.jewelry = UNKNOWN_STATUS;
                m_internalMap.at(h).push_front(c);
            }
            //Add 1 to origin and current pos
            m_currentPos.x += 1;
            m_basePos.x += 1;
        }
    }
    if(m_currentPos.y == 0) {
        if(sensors.north) {
            //Add a row
            std::deque<Case> newRow;
            for(size_t w = 0; w < m_internalMap.at(0).size(); ++w) {
                Case c;
                c.isFloor = (int)w == m_currentPos.x;
                c.dirtLevel = UNKNOWN_STATUS;
                c.jewelry = UNKNOWN_STATUS;
                newRow.push_back(c);
            }
            m_internalMap.push_front(newRow);
            //Add 1 to origin and current pos
            m_currentPos.y += 1;
            m_basePos.y += 1;
        }
    }
    if(m_currentPos.x == (int)m_internalMap.at(0).size()-1) {
        if(sensors.east) {
            //Add a col
            for(size_t h = 0; h < m_internalMap.size(); ++h) {
                Case c;
                c.isFloor = (int)h == m_currentPos.y;
                c.dirtLevel = UNKNOWN_STATUS;
                c.jewelry = UNKNOWN_STATUS;
                m_internalMap.at(h).push_back(c);
            }
        }
    }
    if(m_currentPos.y == (int)m_internalMap.size()-1) {
        if(sensors.south) {
            //Add a row
            std::deque<Case> newRow;
            for(size_t w = 0; w < m_internalMap.at(0).size(); ++w) {
                Case c;
                c.isFloor = (int)w == m_currentPos.x;
                c.dirtLevel = UNKNOWN_STATUS;
                c.jewelry = UNKNOWN_STATUS;
                newRow.push_back(c);
            }
            m_internalMap.push_back(newRow);
        }
    }

    //Verify North/South/East/West
    if(m_currentPos.y > 0) {
        if(sensors.north && !m_internalMap[m_currentPos.y-1][m_currentPos.x].isFloor)
        m_internalMap[m_currentPos.y-1][m_currentPos.x].isFloor = true;
    }
    if(m_currentPos.y < (int)m_internalMap.size()-1) {
        if(sensors.south && !m_internalMap[m_currentPos.y+1][m_currentPos.x].isFloor)
        m_internalMap[m_currentPos.y+1][m_currentPos.x].isFloor = true;
    }
    if(m_currentPos.x > 0) {
        if(sensors.west && !m_internalMap[m_currentPos.y][m_currentPos.x-1].isFloor)
        m_internalMap[m_currentPos.y][m_currentPos.x-1].isFloor = true;
    }
    if(m_currentPos.x < (int)m_internalMap.at(0).size()-1) {
        if(sensors.east && !m_internalMap[m_currentPos.y][m_currentPos.x+1].isFloor)
        m_internalMap[m_currentPos.y][m_currentPos.x+1].isFloor = true;
    }
}

/*SuckWithLevelStrategy::SuckWithLevelStrategy(MapReal& map, const Pos pBase) : Strategy(map, pBase) {
initializeInternalMap();
_suckLevel = 0.2;
}


void SuckWithLevelStrategy::observeAndUpdate() {

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
if(chosenAction.type == Iddle) {
std::cout << "[Vacuum] IDDLE(" << _currentPos.x << ";" << _currentPos.y << ")" << std::endl;
if(_pBase.x == _currentPos.x && _pBase.y == _currentPos.y) {
std::cout << "[Vacuum] is fully charged" << std::endl;
_energy = 100;
}
}
if(chosenAction.type == Suck) {
std::cout << "[Vacuum] Suck(" << _currentPos.x << ";" << _currentPos.y << "); "
<< "D:" << _map.dirtLevel(_currentPos)
<< "J:" << _map.jewelry(_currentPos)
<< std::endl;
_map.setDirtLevel(_currentPos, 0);
_map.setJewelry(_currentPos, 0);
}
if(chosenAction.type == Gather) {
std::cout << "[Vacuum] Gather(" << _currentPos.x << ";" << _currentPos.y << "); "
<< "D:" << _map.dirtLevel(_currentPos)
<< "J:" << _map.jewelry(_currentPos)
<< std::endl;
_map.setJewelry(_currentPos, 0);
}
if(chosenAction.type == GoNorth) {
_energy -= 1.0;
_currentPos.y -= 1;
std::cout << "[Vacuum] MOVE(" << _currentPos.x << ";" << _currentPos.y << "); " << std::endl;
}
if(chosenAction.type == GoSouth) {
_energy -= 1.0;
_currentPos.y += 1;
std::cout << "[Vacuum] MOVE(" << _currentPos.x << ";" << _currentPos.y << "); " << std::endl;
}
if(chosenAction.type == GoWest) {
_energy -= 1.0;
_currentPos.x -= 1;
std::cout << "[Vacuum] MOVE(" << _currentPos.x << ";" << _currentPos.y << "); " << std::endl;
}
if(chosenAction.type == GoEast) {
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
Action chosenAction;
chosenAction.type  = Iddle;
auto x = _currentPos.x;
auto y = _currentPos.y;
//1. Iddle
auto scoreIddle = .0;
if(_pBase.x == x && _pBase.y == y) {
scoreIddle = 100-_energy; //if the energy is low it's better to stay
}
auto currentScore = scoreIddle;
std::cout << "iddle:" << scoreIddle << std::endl;
//2. Suck
auto scoreSuck = -1.0; //We loose 1% of energy
scoreSuck += 10*_internalMap[y][x].dirtLevel;
scoreSuck -= 100*_internalMap[y][x].jewelry;
if(scoreSuck > currentScore) chosenAction.type = Suck;
std::cout << "dirt:" << scoreSuck << std::endl;
//3. Gather
auto scoreGather = -1.0; //We loose 1% of energy
scoreGather += 10*_internalMap[y][x].jewelry;
if(scoreGather > currentScore) chosenAction.type = Gather;
std::cout << "jewel:" << scoreGather << std::endl;
//4. TODO move (prévision 1 case)
//UP
Pos up;
up.x = x;
up.y = y-1;
if(_map.isFloor(up)) {
auto scoreUp = getScoreMove(up);
if(scoreUp > currentScore) chosenAction.type = GoNorth;
std::cout << "Up:" << scoreUp << std::endl;
}
//DOWN
Pos down;
down.x = x;
down.y = y+1;
if(_map.isFloor(down)) {
auto scoreDown = getScoreMove(down);
if(scoreDown > currentScore) chosenAction.type = GoSouth;
std::cout << "Down:" << scoreDown << std::endl;
}
//LEFT
Pos left;
left.x = x-1;
left.y = y;
if(_map.isFloor(left)) {
auto scoreLeft = getScoreMove(left);
if(scoreLeft > currentScore) chosenAction.type = GoWest;
std::cout << "Left:" << scoreLeft << std::endl;
}
//RIGHT
Pos right;
right.x = x+1;
right.y = y;
if(_map.isFloor(right)) {
auto scoreRight = getScoreMove(right);
if(scoreRight > currentScore) chosenAction.type = GoEast;
std::cout << "Right:" << scoreRight << std::endl;
}

return chosenAction;
}
*/
