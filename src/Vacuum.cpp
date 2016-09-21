#include <chrono>
#include <iostream>
#include "Vacuum.h"
#include "SuckWithLevelStrategy.h"

Vacuum::Vacuum(MapReal& map, Pos pBase) {
    _strategy = new SuckWithLevelStrategy(map, pBase);
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
        _strategy->observeAndUpdate();
        _strategy->pickAndExecAction();
        //TODO remove
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
