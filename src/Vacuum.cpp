#include <thread>
#include <chrono>
#include <iostream>

#include "Vacuum.h"
#include "Sensors.h"

/* Public constructors */
Vacuum::Vacuum(std::unique_ptr<Strategy>& strategy, Pos basePosition, MapReal map) :
    m_dBattery(100.),
    m_position(basePosition),
    m_basePosition(basePosition),
    m_strategy(std::move(strategy)),
    m_map(std::make_unique<MapReal>(map))  {
    }

    /* Public methods */
    bool Vacuum::isBusy() {
        return m_currentAction.timer > 0;
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

    void Vacuum::findNextAction(const Sensors& sensors) {
        m_currentAction = m_strategy->findNextAction(sensors);
    }

    void Vacuum::executeCurrentAction(double delta) {
        m_currentAction.timer -= delta;

        if(m_currentAction.timer <= 0) {
            switch(m_currentAction.type) {
            case GoNorth:
                m_position.y -= 1;
                std::cout << "[VACUUM]GONORTH (" << m_position.x << ";" << m_position.y << ")" << std::endl;
                break;
            case GoSouth:
                m_position.y += 1;
                std::cout << "[VACUUM]GOSOUTH (" << m_position.x << ";" << m_position.y << ")" << std::endl;
                break;
            case GoEast:
                m_position.x += 1;
                std::cout << "[VACUUM]GOEAST (" << m_position.x << ";" << m_position.y << ")" << std::endl;
                break;
            case GoWest:
                m_position.x -= 1;
                std::cout << "[VACUUM]GOWEST (" << m_position.x << ";" << m_position.y << ")" << std::endl;
                break;
            case Gather:
                std::cout << "[VACUUM]GATHER (" << m_position.x << ";" << m_position.y << ")" << std::endl;
                m_map->gatherJewelry(m_position);
                break;
            case Suck:
                std::cout << "[VACUUM]SUCKDIRT (" << m_position.x << ";" << m_position.y << ")" << std::endl;
                m_map->suckDirt(m_position);
                break;
            case Iddle:
                std::cout << "[VACUUM]IDDLE (" << m_position.x << ";" << m_position.y << ")" << std::endl;
                if((m_position.x == m_basePosition.x) && (m_position.y == m_basePosition.y)) m_dBattery = 100.;
                break;
            default:
                break;
            }
            m_currentAction.type = Iddle;
        }
    }

    void Vacuum::update(double delta) {
        // Récupérer les données des capteurs

        auto delay = std::chrono::milliseconds(1000);
        std::cout << "[Vacuum]sleep " << std::chrono::duration_cast<std::chrono::milliseconds>(delay).count() << " ms" << std::endl;
        std::this_thread::sleep_for(delay);

        Sensors sensors = observe();

        // Executer une action
        if(!isBusy()) {
            findNextAction(sensors);
        }
        executeCurrentAction(delta);
    }
