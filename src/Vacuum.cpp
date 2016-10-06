#include <thread>
#include <chrono>
#include <iostream>

#include "Vacuum.h"
#include "Settings.h"
#include "Sensors.h"
#include "Console.h"

/* Public constructors */
Vacuum::Vacuum(std::unique_ptr<Strategy>& strategy, Pos basePosition, Map& map) :
    m_dBattery(100.),
    m_position(basePosition),
    m_basePosition(basePosition),
    m_strategy(std::move(strategy)),
    m_map(&map) {
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

    // Logger les données sensibles du robot
    std::ostream& out = Console::out(1);
    out << "Position : " << m_position << std::endl << "Batterie : [";
    int k;
    for(k = 2; k < (int)m_dBattery; k+=2) out << "=";
    for(; k < 100; k+=2) out << " ";
    out << "]" << std::endl;

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

    // Logger l'action choisie
    std::ostream& out = Console::out(1);
    out << "Action : " << action << std::endl;

    return action;
}

void Vacuum::execute(Action action) {
    // Patienter le temps de l'exécution de l'action
    auto delay = std::chrono::milliseconds(int(action.timer * 1000
                / Settings::WORLD_SPEED));
    std::this_thread::sleep_for(delay);

    // Calculer les répercusions de l'action
    switch(action.type) {
    case GoNorth:
        m_dBattery -= 1;
        m_position.y -= 1;
        break;
    case GoSouth:
        m_dBattery -= 1;
        m_position.y += 1;
        break;
    case GoEast:
        m_dBattery -= 1;
        m_position.x += 1;
        break;
    case GoWest:
        m_dBattery -= 1;
        m_position.x -= 1;
        break;
    case Gather:
        m_dBattery -= 1.2;
        m_map->gatherJewelry(m_position);
        break;
    case Suck:
        m_dBattery -= action.timer;
        m_map->suckDirt(m_position, 0.5 * action.timer);
        break;
    case Iddle:
        if(m_position == m_basePosition) {
            m_dBattery = std::min(100., m_dBattery + 2 * action.timer);
        }
        break;
    default:
        break;
    }
}

void Vacuum::update(double delta) {
    // Logger le nouveau tour dans la boucle logique du robot-aspirateur
    std::ostream& out = Console::out(1);
    out << std::endl << "---" << std::endl << std::endl;

    // Si batterie vide l'aspirateur s'arrête
    if(batteryIsEmpty()) {
        out << "[ERROR] Battery is empty. Stop !" << std::endl;
        out << "[ERROR] Wait until a friendly human plugs me..." << std::endl;
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
    return m_dBattery <= 0;
}
