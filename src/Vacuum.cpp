#include <chrono>
#include <iostream>
#include "Vacuum.h"

Vacuum::Vacuum(Map& map, Pos pBase) : _currentPos(pBase), _energy(100), _pBase(pBase), _map(map)  {
    if(!map.isFloor(pBase)) {
        throw "Position de base invalide.";
    }
}

std::thread Vacuum::start() {
    _shouldStop = false;
    return std::thread(&Vacuum::run, this);
}

void Vacuum::stop() {
    _shouldStop = true;
}

void Vacuum::run() {
    while(!_shouldStop) {
        std::cout << "thread vacuum" << std::endl;
        auto delay = std::chrono::milliseconds(1000);
        std::this_thread::sleep_for(delay);
    }
    /*auto startTime = std::chrono::system_clock::now();
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> frameDuration;
    double frameTime;

    while(!_shouldStop) {
        endTime = std::chrono::system_clock::now();
        frameDuration = endTime - startTime;
        startTime = endTime;
        frameTime = endTime.count();
    }*/
}
