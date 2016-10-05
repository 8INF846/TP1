#include "SuckWithLevelStrategy.h"

#include <iostream>
#include <chrono>

SuckWithLevelStrategy::SuckWithLevelStrategy() : Strategy() {
    //Add 1 case
    std::deque<StrCase> origin = std::deque<StrCase>(1);
    m_internalMap.push_back(origin);
    m_basePos.x = 0;
    m_basePos.y = 0;
    m_previousPos.x = -1;
    m_previousPos.y = -1;

    m_mt = std::mt19937(m_rd());
    m_dist = std::uniform_int_distribution<int>(0,1);
}

bool SuckWithLevelStrategy::changeFMaxScore(const float score) {
    if(score > m_fmaxScore || (score == m_fmaxScore && m_dist(m_mt) == 0)) {
        m_fmaxScore = score;
        return true;
    }
    return false;
}

Action SuckWithLevelStrategy::findNextAction(const Sensors& sensors)
{
    Action finalAction;
    finalAction.type = Iddle;
    updateInternalMap(sensors);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0,1);

    auto scoreIddle = (m_currentPos == m_basePos) ?
    100.-sensors.battery : 0;
    if(sensors.battery < 30) scoreIddle += (m_currentPos == m_basePos) ? std::pow(30-sensors.battery, 2) : 0;
    m_fmaxScore = scoreIddle;

    auto scoreSuck = -1.0; //(loose 1 of energy)
    scoreSuck += 90*sensors.dirt;
    if(sensors.dirt > 0.1) scoreSuck += 200*sensors.dirt;
    scoreSuck -= 200*sensors.jewelry; //If jewelry on the case
    scoreSuck -= sensors.battery < 30 ? std::pow(30-sensors.battery, 2) : 0;
    if(changeFMaxScore(scoreSuck)) finalAction.type = Suck;

    auto scoreGather = -1.0; //(loose 1 of energy)
    scoreGather += 200*sensors.jewelry; //If jewelry on the case
    scoreGather -= sensors.battery < 30 ? std::pow(30-sensors.battery, 2) : 0;
    if(changeFMaxScore(scoreGather)) finalAction.type = Gather;

    //Test if a new case is discovered
    auto scoreMoveNorth = 1.0;
    if(sensors.north) {
        if(m_internalMap[m_currentPos.y-1][m_currentPos.x].dirtLevel == UNKNOWN_STATUS) scoreMoveNorth += getScoreDiscoverCase();
        if(changeFMaxScore(scoreMoveNorth)) finalAction.type = GoNorth;
    }
    auto scoreMoveSouth = 1.0;
    if(sensors.south) {
        if(m_internalMap[m_currentPos.y+1][m_currentPos.x].dirtLevel == UNKNOWN_STATUS) scoreMoveSouth += getScoreDiscoverCase();
        if(changeFMaxScore(scoreMoveSouth)) finalAction.type = GoSouth;
    }
    auto scoreMoveEast = 1.0;
    if(sensors.east) {
        if(m_internalMap[m_currentPos.y][m_currentPos.x+1].dirtLevel == UNKNOWN_STATUS) scoreMoveEast += getScoreDiscoverCase();
        if(changeFMaxScore(scoreMoveEast)) finalAction.type = GoEast;
    }
    auto scoreMoveWest = 1.0;
    if(sensors.west) {
        if(m_internalMap[m_currentPos.y][m_currentPos.x-1].dirtLevel == UNKNOWN_STATUS) scoreMoveWest += getScoreDiscoverCase();
        if(changeFMaxScore(scoreMoveWest)) finalAction.type = GoWest;
    }


    //Try to find another unvisited case
    //And get oldest and youngest visit
    time_t oldestVisit = 0;
    Pos oldestCasePos;
    oldestCasePos.x=-1;
    oldestCasePos.y=-1;
    for(size_t h = 0; h < m_internalMap.size(); ++h) {
        for(size_t w = 0; w < m_internalMap[0].size(); ++w) {
            //Test if a case is not visited
            if(m_internalMap[h][w].isFloor && m_internalMap[h][w].dirtLevel == UNKNOWN_STATUS) {
                if((int)h < m_currentPos.y && sensors.north) {
                    scoreMoveNorth += 70;
                    if(changeFMaxScore(scoreMoveNorth)) finalAction.type = GoNorth;
                }
                if((int)h > m_currentPos.y && sensors.south) {
                    scoreMoveSouth += 70;
                    if(changeFMaxScore(scoreMoveSouth)) finalAction.type = GoSouth;
                }
                if((int)w < m_currentPos.x && sensors.west) {
                    scoreMoveWest += 70;
                    if(changeFMaxScore(scoreMoveWest)) finalAction.type = GoWest;
                }
                if((int)w > m_currentPos.x && sensors.east) {
                    scoreMoveEast += 70;
                    if(changeFMaxScore(scoreMoveEast)) finalAction.type = GoEast;
                }
            }
            //Find the oldest case
            if(m_internalMap[h][w].isFloor) {
                auto difftime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - m_internalMap[h][w].lastVisit;
                if(difftime > oldestVisit) {
                    oldestCasePos.x = w;
                    oldestCasePos.y = h;
                    oldestVisit = difftime;
                }
            }
        }
    }

    //Remove score to avoid the last case visited
    if(m_previousPos.x != -1 && m_previousPos.y != -1) {
        if(m_previousPos.y < m_currentPos.y && sensors.north) {
            scoreMoveNorth -= 10;
        }

        if(m_previousPos.y > m_currentPos.y && sensors.south) {
            scoreMoveSouth -= 10;
        }

        if(m_previousPos.x < m_currentPos.x && sensors.west) {
            scoreMoveWest -= 10;
        }
        if(m_previousPos.x > m_currentPos.x && sensors.east) {
            scoreMoveEast -= 10;
        }
    }
    m_previousPos.x = m_currentPos.x;
    m_previousPos.y = m_currentPos.y;

    //Add score if we want to visit the oldest case
    if(oldestCasePos.x != -1 && oldestCasePos.y != -1) {
        if(oldestCasePos.y < m_currentPos.y && sensors.north) {
            scoreMoveNorth += 80;
            if(changeFMaxScore(scoreMoveNorth)) finalAction.type = GoNorth;
        }

        if(oldestCasePos.y > m_currentPos.y && sensors.south) {
            scoreMoveSouth += 80;
            if(changeFMaxScore(scoreMoveSouth)) finalAction.type = GoSouth;
        }

        if(oldestCasePos.x < m_currentPos.x && sensors.west) {
            scoreMoveWest += 80;
            if(changeFMaxScore(scoreMoveWest)) finalAction.type = GoWest;
        }
        if(oldestCasePos.x > m_currentPos.x && sensors.east) {
            scoreMoveEast += 80;
            if(changeFMaxScore(scoreMoveEast)) finalAction.type = GoEast;
        }

        //Avoid blocking case
        if((oldestCasePos.y > m_currentPos.y && !sensors.south)
        || (oldestCasePos.y < m_currentPos.y && !sensors.north)) {
            if((sensors.west && !sensors.east) || (sensors.west && sensors.east
            && m_internalMap[m_currentPos.y][m_currentPos.x-1].lastVisit
            < m_internalMap[m_currentPos.y][m_currentPos.x+1].lastVisit)) {
                scoreMoveWest += 60;
                if(changeFMaxScore(scoreMoveWest)) finalAction.type = GoWest;
            } else if(sensors.east) {
                scoreMoveEast += 60;
                if(changeFMaxScore(scoreMoveEast)) finalAction.type = GoEast;
            }
        }

        if((oldestCasePos.x > m_currentPos.x && !sensors.east)
        || (oldestCasePos.x < m_currentPos.x && !sensors.west)) {
            if((sensors.north && !sensors.south) || (sensors.north && sensors.south
            && m_internalMap[m_currentPos.y-1][m_currentPos.x].lastVisit
            < m_internalMap[m_currentPos.y+1][m_currentPos.x].lastVisit)) {
                scoreMoveNorth += 60;
                if(changeFMaxScore(scoreMoveNorth)) finalAction.type = GoNorth;
            } else if(sensors.south) {
                scoreMoveSouth += 60;
                if(changeFMaxScore(scoreMoveSouth)) finalAction.type = GoSouth;
            }
        }
    }

    //If battery is low, add score to return to base
    unsigned int futureBattery = sensors.battery - std::abs(m_currentPos.x - m_basePos.x) - std::abs(m_currentPos.y - m_basePos.y);
    if(m_basePos.y < m_currentPos.y && sensors.north) {
        scoreMoveNorth += futureBattery < 30 ? std::pow(30-futureBattery, 2) : 0;
        if(changeFMaxScore(scoreMoveNorth)) finalAction.type = GoNorth;
    }
    if(m_basePos.y > m_currentPos.y && sensors.south) {
        scoreMoveSouth += futureBattery < 30 ? std::pow(30-futureBattery, 2) : 0;
        if(changeFMaxScore(scoreMoveSouth)) finalAction.type = GoSouth;
    }
    if(m_basePos.x < m_currentPos.x && sensors.west) {
        scoreMoveWest += futureBattery < 30 ? std::pow(30-futureBattery, 2) : 0;
        if(changeFMaxScore(scoreMoveWest)) finalAction.type = GoWest;
    }
    if(m_basePos.x > m_currentPos.x && sensors.east) {
        scoreMoveEast += futureBattery < 30 ? std::pow(30-futureBattery, 2) : 0;
        if(changeFMaxScore(scoreMoveEast)) finalAction.type = GoEast;
    }

    simulateInternalAction(finalAction.type);


    std::cout << "Score:" << m_fmaxScore <<  std::endl;
    std::cout << "Battery:" << sensors.battery <<  std::endl;
    std::cout << "Iddle:" << scoreIddle <<  std::endl;
    std::cout << "ScoreNorth:" << scoreMoveNorth <<  std::endl;
    std::cout << "ScoreSouth:" << scoreMoveSouth <<  std::endl;
    std::cout << "ScoreWest:" << scoreMoveWest<<  std::endl;
    std::cout << "ScoreEast:" << scoreMoveEast<<  std::endl;
    printInternalMap(oldestCasePos);
    return finalAction;
}

void SuckWithLevelStrategy::simulateInternalAction(ActionType action) {
    //Update internal state
    switch (action)
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
    case Gather:
        m_internalMap[m_currentPos.y][m_currentPos.x].jewelry = 0;
        break;
    case Suck:
        m_internalMap[m_currentPos.y][m_currentPos.x].dirtLevel = 0;
        break;
    default:
        break;
    }
}

void SuckWithLevelStrategy::printInternalMap(Pos oldest) {
    std::cout << std::endl;
    for(size_t h = 0; h < m_internalMap.size(); ++h) {
        for(size_t w = 0; w < m_internalMap[0].size(); ++w) {
            if(m_internalMap[h][w].isFloor) {
                std::cout << "| d:" << (m_internalMap[h][w].dirtLevel*10)
                << " j:" << m_internalMap[h][w].jewelry;
                if(m_currentPos.x == (int)w && m_currentPos.y == (int)h) {
                    std::cout << " (X)";
                } else {
                    std::cout << " (_)";
                }

                if(m_basePos.x == (int)w && m_basePos.y == (int)h) {
                    std::cout << " C ";
                } else {
                    std::cout << "   ";
                }

                if(oldest.x == (int)w && oldest.y == (int)h) {
                    std::cout << " O ";
                } else {
                    std::cout << "   ";
                }
            } else {
                std::cout << "|==================";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


float SuckWithLevelStrategy::getScoreDiscoverCase() {
    return 100.;
}

void SuckWithLevelStrategy::updateInternalMap(const Sensors& sensors) {
    m_internalMap[m_currentPos.y][m_currentPos.x].isFloor = true;
    m_internalMap[m_currentPos.y][m_currentPos.x].dirtLevel = sensors.dirt;
    m_internalMap[m_currentPos.y][m_currentPos.x].jewelry = sensors.jewelry;
    m_internalMap[m_currentPos.y][m_currentPos.x].lastVisit = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    if(m_currentPos.x == 0) {
        if(sensors.west) {
            //Add a col
            for(size_t h = 0; h < m_internalMap.size(); ++h) {
                StrCase c;
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
            std::deque<StrCase> newRow;
            for(size_t w = 0; w < m_internalMap.at(0).size(); ++w) {
                StrCase c;
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
                StrCase c;
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
            std::deque<StrCase> newRow;
            for(size_t w = 0; w < m_internalMap.at(0).size(); ++w) {
                StrCase c;
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
