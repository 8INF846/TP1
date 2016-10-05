#include <thread>
#include <chrono>
#include <iostream>

#include "Vacuum.h"
#include "Sensors.h"

/* Public constructors */
Vacuum::Vacuum(std::unique_ptr<Strategy>& strategy, Pos basePosition, Map& map, unsigned int speed) :
    m_dBattery(100.),
    m_position(basePosition),
    m_basePosition(basePosition),
    m_strategy(std::move(strategy)),
    m_map(&map),
    m_uiSpeed(speed)
    {
}

/* Public methods */
bool Vacuum::isBusy() {
    return m_currentAction.timer > 0;
}

Pos Vacuum::position() const {
    return m_position;
}

Pos Vacuum::basePosition() const {
    return m_basePosition;
}

/* Private methods */
Sensors Vacuum::observe() const {
    Sensors sensors;
    // North
    Pos position = m_position;
    position.y -= 1;
    sensors.north = m_map->isFloor(position);
    // South
    position.y += 2;
    sensors.south = m_map->isFloor(position);
    // Est
    position.y -= 1;
    position.x += 1;
    sensors.east = m_map->isFloor(position);
    // West
    position.x -= 2;
    sensors.west = m_map->isFloor(position);
    // Dirt
    position.x += 1;
    sensors.dirt = m_map->dirtLevel(position);
    // Jewelry
    sensors.jewelry = m_map->jewelry(position);
    // Battery
    sensors.battery = m_dBattery;
    // Charging
    sensors.charging = m_position == m_basePosition;

    return sensors;
}

Action Vacuum::findNextAction(const Sensors& sensors) {
    // Demander à la stratégie l'action à réaliser
    Action action = m_strategy->findNextAction(sensors);

    // S'assurer de la cohérence de l'action donnée
    switch(action.type) {
    case GoNorth:
    case GoSouth:
    case GoEast:
    case GoWest:
        action.timer = 1;
        break;
    case Gather:
        action.timer = 1.2;
    case Suck:
    case Iddle:
        action.timer = std::max(0.1, action.timer);
        break;
    default:
        action.timer = 0.1;
    }
    action.timer /= m_uiSpeed;
    return action;
}

void Vacuum::execute(Action action) {
    // Patienter le temps de l'exécution de l'action
    auto delay = std::chrono::milliseconds(int(action.timer * 1000));
    std::this_thread::sleep_for(delay);

    // Calculer les répercusions de l'action
    switch(action.type) {
    case GoNorth:
        m_dBattery -= 1;
        m_position.y -= 1;
        std::cout << "[VACUUM]GONORTH" << m_position << std::endl;
        break;
    case GoSouth:
        m_dBattery -= 1;
        m_position.y += 1;
        std::cout << "[VACUUM]GOSOUTH" << m_position << std::endl;
        break;
    case GoEast:
        m_dBattery -= 1;
        m_position.x += 1;
        std::cout << "[VACUUM]GOEAST" << m_position << std::endl;
        break;
    case GoWest:
        m_dBattery -= 1;
        m_position.x -= 1;
        std::cout << "[VACUUM]GOWEST" << m_position << std::endl;
        break;
    case Gather:
        m_dBattery -= 1.2;
        std::cout << "[VACUUM]GATHER" << m_position << std::endl;
        m_map->gatherJewelry(m_position);
        break;
    case Suck:
        m_dBattery -= action.timer;
        m_map->suckDirt(m_position, 15 * action.timer);
        std::cout << "[VACUUM]SUCKDIRT" << m_position << std::endl;
        break;
    case Iddle:
        std::cout << "[VACUUM]IDDLE" << m_position << std::endl;
        if(m_position == m_basePosition) {
            m_dBattery = 100.0;
        }
        break;
    default:
        std::cout << action.type << "##" << std::endl;
        break;
    }
}

void Vacuum::update(double delta) {
    // Si batterie vide l'aspirateur s'arrête
    if(batteryIsEmpty()) {
        std::cout << "[Vacuum]BATTERY IS EMPTY. STOP!" << std::endl;
        stop();
    }

    // Observer l'environnement
    Sensors sensors = observe();

    // Trouver l'action à exécuter
    Action action = findNextAction(sensors);

    // Exécuter l'action choisie
    execute(action);
}

bool Vacuum::batteryIsEmpty() {
    return m_dBattery == 0;
}
