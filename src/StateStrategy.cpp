#include "StateStrategy.h"
#include "StateStrategyStates.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <string>
#include <ctime>
#include "Console.h"

StateStrategy::StateStrategy(): m_position(0, 0), m_basePosition(0, 0) {
    m_state = GoToBaseAndChargeBatteryState::getInstance();

    std::deque<StrCase> firstColumn;
    firstColumn.push_back(StrCase(true, true));
    m_map.push_back(firstColumn);
}

Action StateStrategy::findNextAction(const Sensors& sensors) {
    // Mise à jour de la carte interne de la stratégie
    updateInternalMap(sensors);
    displayInternalMap();

    // Mise à jour de l'état intern du robot-aspirateur
    m_battery = sensors.battery;

    // Recherche de l'action à effectuer
    Action action;
    while(action.type == None) {
        action = m_state->execute(this);
    }
    return action;
}

void StateStrategy::setState(State* state) {
    m_state = state;
}

void StateStrategy::go(ActionType direction) {
    switch(direction) {
    case GoNorth:
        m_position.y -= 1;
        break;
    case GoSouth:
        m_position.y += 1;
        break;
    case GoWest:
        m_position.x -= 1;
        break;
    case GoEast:
        m_position.x += 1;
        break;
    default:
        break;
    }
}

bool StateStrategy::batteryFull() const {
    std::ostream& out = Console::out(4);
    out << "<batteryFull?>" << std::endl;
    return m_battery > 99.9;
}

double StateStrategy::batteryLevel() const {
    std::ostream& out = Console::out(4);
    out << "<battery?>" << std::endl;
    return m_battery;
}

double StateStrategy::smartBatteryLevel() {
    std::ostream& out = Console::out(4);
    out << "<batterySmart?>" << std::endl;
    auto path = pathTo(m_basePosition);
    return m_battery - 2 - 1 * path.size();
}

int StateStrategy::jewelry() const {
    std::ostream& out = Console::out(4);
    out << "<jewelry?>" << std::endl;
    return m_map[m_position.y][m_position.x].jewelry;
}

double StateStrategy::dirtLevel() const {
    std::ostream& out = Console::out(4);
    out << "<dirt?>" << std::endl;
    return m_map[m_position.y][m_position.x].dirtLevel;
}

bool StateStrategy::isOnBase() const {
    return m_position == m_basePosition;
}

ActionType StateStrategy::actionTypeToBase() {
    auto path = pathTo(m_basePosition);
    return path[0];
}

ActionType StateStrategy::actionTypeToLatestVisitedCase() {
    auto p = positionOfLatestVisitedCase();
    auto path = pathTo(p);
    if(path.size() == 0)
        throw std::string("path.size() == 0");
    return path[0];
}

double distance(const Pos& p1, const Pos& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

double StateStrategy::distanceTo(const Pos& position) const {
    return distance(position, m_position);
}

Pos StateStrategy::positionOfLatestVisitedCase() const {
    auto nowClock = std::chrono::system_clock::now();
    auto latestTime = std::chrono::system_clock::to_time_t(nowClock);

    double minDist = 0;
    Pos position;
    for(unsigned int y = 0; y < m_map.size(); y++) {
        for(unsigned int x = 0; x < m_map[0].size(); x++) {
            auto lastVisit = m_map[y][x].lastVisit;
            Pos pos(x, y);
            double dist = distanceTo(pos);
            if(m_map[y][x].isFloor && (lastVisit < latestTime || (lastVisit == latestTime && dist < minDist))) {
                latestTime = lastVisit;
                position = pos;
                minDist = dist;
            }
        }
    }
    return position;
}

bool isInSet(Pos pos, std::vector<Pos>& set) {
    for(auto it = set.begin(); it != set.end(); it++) {
        if(*it == pos) {
            return true;
        }
    }
    return false;
}

std::vector<Pos> findNeighbors(Pos pos, std::deque<std::deque<StrCase>>& map, std::vector<Pos>& closedSet) {
    std::vector<Pos> neighbors;
    if(pos.x > 0 && map[pos.y][pos.x-1].isFloor) {
        Pos tmp(pos.x-1, pos.y);
        if(!isInSet(tmp, closedSet)) {
            neighbors.push_back(tmp);
        }
    }
    if(pos.x < map[0].size()-1 && map[pos.y][pos.x+1].isFloor) {
        Pos tmp(pos.x+1, pos.y);
        if(!isInSet(tmp, closedSet)) {
            neighbors.push_back(tmp);
        }
    }
    if(pos.y > 0 && map[pos.y-1][pos.x].isFloor) {
        Pos tmp(pos.x, pos.y-1);
        if(!isInSet(tmp, closedSet)) {
            neighbors.push_back(tmp);
        }
    }
    if(pos.y < map.size()-1 && map[pos.y+1][pos.x].isFloor) {
        Pos tmp(pos.x, pos.y+1);
        if(!isInSet(tmp, closedSet)) {
            neighbors.push_back(tmp);
        }
    }
    return neighbors;
}

std::vector<ActionType> StateStrategy::constructPath(Pos target, Pos source) {
    std::ostream& out = Console::out(4);
    std::deque<Pos> posPath;
    posPath.push_back(target);
    Pos current = target;
    std::vector<Pos> fakeClosedSet;
    while(current != source) {
        current = m_map[current.y][current.x].cameFrom;
        posPath.push_front(current);
    }
    std::vector<ActionType> path;
    auto it = posPath.begin();
    auto last = it;
    it++;
    while(it != posPath.end()) {
        if(it->x < last->x) {
            path.push_back(GoWest);
        }
        else if(it->x > last->x) {
            path.push_back(GoEast);
        }
        else if(it->y < last->y) {
            path.push_back(GoNorth);
        }
        else {
            path.push_back(GoSouth);
        }
        last = it;
        it++;
    }
    out << "5";
    return path;
}

std::vector<ActionType> StateStrategy::pathTo(const Pos& target) {
    /* This algorithm is an implementation of A* following the pseudo-code
     * described on wikipedia's page "A* algorithm"
     */
    std::ostream& out = Console::out(4);
    out << "0";

    Pos source = m_position;
    std::vector<ActionType> path;
    for(unsigned int y = 0; y < m_map.size(); y++) {
        for(unsigned int x = 0; x < m_map[0].size(); x++) {
            Pos p(x, y);
            m_map[y][x].cameFrom = Pos(-1, -1);
            m_map[y][x].gScore = m_map.size() * m_map[0].size();
            m_map[y][x].fScore = distance(source, p) + distance(p, target);
        }
    }
    m_map[source.y][source.x].gScore = 0;

    std::vector<Pos> openSet;
    openSet.push_back(source);
    std::vector<Pos> closedSet;

    while(openSet.size() != 0) {
        out << "1";
        // Pos with lowest fScore in openSet
        auto currentIt = openSet.begin();
        for(auto it = openSet.begin(); it != openSet.end(); it++) {
            if(m_map[it->y][it->x].fScore <
                    m_map[currentIt->y][currentIt->x].fScore) {
                currentIt = it;
            }
        }
        Pos current = *currentIt;
        // Arrived at goal
        if(current == target) {
            out << "2";
            return constructPath(current, source);
        }
        // Remove current node from openSet
        openSet.erase(currentIt);
        closedSet.push_back(current);
        // Find neighbors not in closedSet
        std::vector<Pos> neighbors = findNeighbors(current, m_map, closedSet);
        for(auto it = neighbors.begin(); it != neighbors.end(); it++) {
            auto t_gScore = m_map[current.y][current.x].gScore + distance(current, *it);
            if(!isInSet(*it, openSet)) {
                openSet.push_back(*it);
            }
            if(t_gScore < m_map[it->y][it->x].gScore) {
                m_map[it->y][it->x].cameFrom = current;
                m_map[it->y][it->x].gScore = t_gScore;
                m_map[it->y][it->x].fScore = t_gScore + distance(*it, target);
            }
        }
    }
    throw std::string("Path not found");
}

void StateStrategy::displayInternalMap() {
    std::ostream& out = Console::out(3);
    for(unsigned int y = 0; y < m_map.size(); y++) {
        for(unsigned int x = 0; x < m_map[0].size(); x++) {
            out << m_map[y][x];
        }
        out << std::endl;
    }
}

void StateStrategy::updateInternalMap(const Sensors& sensors) {
    auto now = std::chrono::system_clock::now();

    // Mise à jour de la case courante
    m_map[m_position.y][m_position.x] = StrCase(true, sensors.dirt,
            sensors.jewelry, std::chrono::system_clock::to_time_t(now));

    // Ajout d'une colonne à l'Ouest si nécessaire
    if(m_position.x == 0 && sensors.west) {
        for(unsigned int y = 0; y < m_map.size(); y++) {
            m_map[y].emplace_front(m_position.y == y, m_position.y == y);
        }
        // Recalculer les positions de l'aspirateur et de la base
        m_position.x += 1;
        m_basePosition.x += 1;
    }
    // Ajout d'une ligne au Nord si nécessaire
    if(m_position.y == 0 && sensors.north) {
        std::deque<StrCase> newRow;
        m_map.emplace_back();
        for(unsigned int x = 0; x < m_map[0].size(); x++) {
            newRow.emplace_back(m_position.x == x, m_position.x == x);
        }
        m_map.push_front(newRow);
        // Recalculer les positions de l'aspirateur et de la base
        m_position.y += 1;
        m_basePosition.y += 1;
    }
    // Ajout d'une colonne à l'Est si nécessaire
    if(m_position.x == m_map[0].size()-1 && sensors.east) {
        for(unsigned int y = 0; y < m_map.size(); y++) {
            m_map[y].emplace_back(m_position.y == y, m_position.y == y);
        }
    }
    // Ajout d'une ligne au Sud si nécessaire
    if(m_position.y == m_map.size()-1 && sensors.south) {
        std::deque<StrCase> newRow;
        for(unsigned int x = 0; x < m_map[0].size(); x++) {
            newRow.emplace_back(m_position.x == x, m_position.x == x);
        }
        m_map.push_back(newRow);
    }

    // Update North/south/east/west
    if(m_position.x > 0) {
        m_map[m_position.y][m_position.x-1].status = true;
        m_map[m_position.y][m_position.x-1].isFloor = sensors.west;
    }
    if(m_position.y > 0) {
        m_map[m_position.y-1][m_position.x].status = true;
        m_map[m_position.y-1][m_position.x].isFloor = sensors.north;
    }
    if(m_position.x < m_map[0].size()-1) {
        m_map[m_position.y][m_position.x+1].status = true;
        m_map[m_position.y][m_position.x+1].isFloor = sensors.east;
    }
    if(m_position.y < m_map.size()-1) {
        m_map[m_position.y+1][m_position.x].status = true;
        m_map[m_position.y+1][m_position.x].isFloor = sensors.south;
    }
}
