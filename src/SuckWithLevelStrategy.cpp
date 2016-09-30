#include "SuckWithLevelStrategy.h"

#include <iostream>
#include <random>

SuckWithLevelStrategy::SuckWithLevelStrategy() : Strategy() {
    //Add 1 case
    std::deque<Case> origin = std::deque<Case>(1);
    m_internalMap.push_back(origin);
}

Action SuckWithLevelStrategy::findNextAction(const Sensors& sensors)
{
    //TODO show UI
    Action finalAction;
    updateInternalMap(sensors);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0,1);

    auto scoreIddle = getScoreIddle();
    auto maxScore = scoreIddle;

    auto scoreSuck = -1.0; //(loose 1 of energy)
    scoreSuck += 100*sensors.dirt;
    if(sensors.dirt > 0.2) scoreSuck += 100*sensors.dirt;
    scoreSuck -= 100*sensors.jewelry; //If jewelry on the case
    scoreSuck -= sensors.battery < 30 ? std::pow(30-sensors.battery, 2) : 0;
    if(scoreSuck > maxScore || (scoreSuck == maxScore && dist(mt) == 0)) {
        finalAction.type = Suck;
    }

    auto scoreGather = -1.0; //(loose 1 of energy)
    scoreGather += 100*sensors.jewelry; //If jewelry on the case
    scoreGather -= sensors.battery < 30 ? std::pow(30-sensors.battery, 2) : 0;
    if(scoreGather > maxScore || (scoreGather == maxScore && dist(mt) == 0)) {
        finalAction.type = Gather;
    }

    auto scoreMoveNorth = 0.0;
    if(sensors.north) {
        if(m_internalMap[m_currentPos.y-1][m_currentPos.x].dirtLevel == UNKNOWN_STATUS) scoreMoveNorth += getScoreDiscoverCase();

        if(scoreMoveNorth > maxScore || (scoreMoveNorth == maxScore && dist(mt) == 0)) {
            finalAction.type = GoNorth;
            maxScore = scoreMoveNorth;
        }
    }

    auto scoreMoveSouth = 0.0;
    if(sensors.south) {
        if(m_internalMap[m_currentPos.y+1][m_currentPos.x].dirtLevel == UNKNOWN_STATUS) scoreMoveSouth += getScoreDiscoverCase();

        if(scoreMoveSouth > maxScore || (scoreMoveSouth == maxScore && dist(mt) == 0)) {
            finalAction.type = GoSouth;
            maxScore = scoreMoveSouth;
        }
    }

    auto scoreMoveEast = 0.0;
    if(sensors.east) {
        if(m_internalMap[m_currentPos.y][m_currentPos.x+1].dirtLevel == UNKNOWN_STATUS) scoreMoveEast += getScoreDiscoverCase();

        if(scoreMoveEast > maxScore || (scoreMoveEast == maxScore && dist(mt) == 0)) {
            finalAction.type = GoEast;
            maxScore = scoreMoveEast;
        }
    }

    auto scoreMoveWest = 0.0;
    if(sensors.west) {
        if(m_internalMap[m_currentPos.y][m_currentPos.x-1].dirtLevel == UNKNOWN_STATUS) scoreMoveWest += getScoreDiscoverCase();

        if(scoreMoveWest > maxScore || (scoreMoveWest == maxScore && dist(mt) == 0)) {
            finalAction.type = GoWest;
            maxScore = scoreMoveWest;
        }
    }


    time_t oldestVisit = 0;
    Pos oldestCasePos;
    oldestCasePos.x=-1;
    oldestCasePos.y=-1;
    for(size_t h = 0; h < m_internalMap.size(); ++h) {
        for(size_t w = 0; w < m_internalMap[0].size(); ++w) {
            //If a case is unknown!
            if(m_internalMap[h][w].isFloor && m_internalMap[h][w].dirtLevel == UNKNOWN_STATUS) {
                if((int)h < m_currentPos.y && sensors.north) {
                    scoreMoveNorth += 70;
                    if(scoreMoveNorth > maxScore || (scoreMoveNorth == maxScore && dist(mt) == 0)) {
                        finalAction.type = GoNorth;
                        maxScore = scoreMoveNorth;
                    }
                }
                if((int)h > m_currentPos.y && sensors.south) {
                    scoreMoveSouth += 70;
                    if(scoreMoveSouth > maxScore || (scoreMoveSouth == maxScore && dist(mt) == 0)) {
                        finalAction.type = GoSouth;
                        maxScore = scoreMoveSouth;
                    }
                }
                if((int)w < m_currentPos.x && sensors.west) {
                    scoreMoveWest += 70;
                    if(scoreMoveWest > maxScore || (scoreMoveWest == maxScore && dist(mt) == 0)) {
                        finalAction.type = GoWest;
                        maxScore = scoreMoveWest;
                    }
                }
                if((int)w > m_currentPos.x && sensors.east) {
                    scoreMoveEast += 70;
                    if(scoreMoveEast > maxScore || (scoreMoveEast == maxScore && dist(mt) == 0)) {
                        finalAction.type = GoEast;
                        maxScore = scoreMoveEast;
                    }
                }
            }

            auto difftime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - m_internalMap[h][w].lastVisit;
            if(difftime > oldestVisit) {
                oldestCasePos.x = w;
                oldestCasePos.y = h;
            }
        }
    }

    //oldestVisit
    if(oldestCasePos.x != -1 && oldestCasePos.y != -1) {
        if(oldestCasePos.y < m_currentPos.y && sensors.north) {
            scoreMoveNorth += 30;
            if(scoreMoveNorth > maxScore || (scoreMoveNorth == maxScore && dist(mt) == 0)) {
                finalAction.type = GoNorth;
                maxScore = scoreMoveNorth;
            }
        }
        if(oldestCasePos.y > m_currentPos.y && sensors.south) {
            scoreMoveSouth += 30;
            if(scoreMoveSouth > maxScore || (scoreMoveSouth == maxScore && dist(mt) == 0)) {
                finalAction.type = GoSouth;
                maxScore = scoreMoveSouth;
            }
        }
        if(oldestCasePos.x < m_currentPos.x && sensors.west) {
            scoreMoveWest += 30;
            if(scoreMoveWest > maxScore || (scoreMoveWest == maxScore && dist(mt) == 0)) {
                finalAction.type = GoWest;
                maxScore = scoreMoveWest;
            }
        }
        if(oldestCasePos.x > m_currentPos.x && sensors.east) {
            scoreMoveEast += 30;
            if(scoreMoveEast > maxScore || (scoreMoveEast == maxScore && dist(mt) == 0)) {
                finalAction.type = GoEast;
                maxScore = scoreMoveEast;
            }
        }
    }

    //If battery is low
    unsigned int futureBattery = sensors.battery + std::abs(m_currentPos.x - m_basePos.x) + std::abs(m_currentPos.y - m_basePos.y);
    if(m_basePos.y < m_currentPos.y && sensors.north) {
        scoreMoveNorth += futureBattery < 30 ? std::pow(30-futureBattery, 2) : 0;
        if(scoreMoveNorth > maxScore || (scoreMoveNorth == maxScore && dist(mt) == 0)) {
            finalAction.type = GoNorth;
            maxScore = scoreMoveNorth;
        }
    }
    if(m_basePos.y > m_currentPos.y && sensors.south) {
        scoreMoveSouth += futureBattery < 30 ? std::pow(30-futureBattery, 2) : 0;
        if(scoreMoveSouth > maxScore || (scoreMoveSouth == maxScore && dist(mt) == 0)) {
            finalAction.type = GoSouth;
            maxScore = scoreMoveSouth;
        }
    }
    if(m_basePos.x < m_currentPos.x && sensors.west) {
        scoreMoveWest += futureBattery < 30 ? std::pow(30-futureBattery, 2) : 0;
        if(scoreMoveWest > maxScore || (scoreMoveWest == maxScore && dist(mt) == 0)) {
            finalAction.type = GoWest;
            maxScore = scoreMoveWest;
        }
    }
    if(m_basePos.x > m_currentPos.x && sensors.east) {
        scoreMoveEast += futureBattery < 30 ? std::pow(30-futureBattery, 2) : 0;
        if(scoreMoveEast > maxScore || (scoreMoveEast == maxScore && dist(mt) == 0)) {
            finalAction.type = GoEast;
            maxScore = scoreMoveEast;
        }
    }

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
    m_internalMap[m_currentPos.y][m_currentPos.x].lastVisit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    m_fenergy = sensors.battery;

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
