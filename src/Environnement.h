#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

//#include <thread>
#include <string>

#include "MapReal.h"

class Environnement {
public:
    Environnement(const std::string& filename);
    void initializeMap(const std::string& filename);
    void start();

private:
    MapReal* _map;
};


#endif
