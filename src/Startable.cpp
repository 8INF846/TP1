#include "Startable.h"

#include <chrono>
#include <iostream>

std::thread Startable::start() {
    m_bShouldStop = false;
    return std::thread(&Startable::run, this);
}

void Startable::stop() {
    m_bShouldStop = true;
}

void Startable::run() {
    auto startTime = std::chrono::system_clock::now();
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> deltaDuration;
    double delta;

    while(!m_bShouldStop) {
        endTime = std::chrono::system_clock::now();
        deltaDuration = endTime - startTime;
        delta = deltaDuration.count();
        startTime = endTime;

        std::cout << "Startable" << std::endl;
        update(delta);
    }
}
