#include <thread>
#include <chrono>
#include "Vacuum.h"

Vacuum::Vacuum(std::shared_ptr<Strategy> strategy, Map& map, int iBase, int jBase) :
        _strategy(strategy), _map(map), _i(iBase), _j(jBase), _energy(100), _iBase(iBase), _jBase(jBase) {
    if(!map.isFloor(iBase, jBase)) {
        throw "Position de base invalide.";
    }
}

void Vacuum::start() {
    _shouldStop = false;
    std::thread t(&Vacuum::run, this);
}

void Vacuum::stop() {
    _shouldStop = true;
}

void Vacuum::run() {
    auto startTime = std::chrono::system_clock::now();
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> frameDuration;
    double frameTime;

    while(!_shouldStop) {
        endTime = std::chrono::system_clock::now();
        frameDuration = endTime - startTime;
        startTime = endTime;
        frameTime = endTime.count();

        if(!_curentAction.isOver()) {
            _currentAction.execute(frameTime);
        }
        else {
            _currentAction = _strategy.execute(_proxy);
        }
    }
}
