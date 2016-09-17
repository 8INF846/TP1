#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include <string>

#include "MapReal.h"

class Environnement {
public:
    Environnement(const std::string& filename);
    void initializeMap(const std::string& filename);
    void start();

private:
    MapReal* _map;
    std::thread _threadMap;
};


#endif
