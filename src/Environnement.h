#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include <string>

#include "MapReal.h"
#include "Vacuum.h"

class Environnement {
public:
    Environnement(const std::string& filename);
    void start();

private:
    void initializeMap(const std::string& filename);
    void initializeVacuum();
    //TODO unique ptr
    MapReal* _map;
    Vacuum* _vacuum;
    std::thread _threadMap;
    std::thread _threadVacuum;
};


#endif
