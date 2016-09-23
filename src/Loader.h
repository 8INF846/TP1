#ifndef LOADER_H
#define LOADER_H

#include <string>

#include "Map.h"
#include "Vacuum.h"

class Environment {
public:
    static Map loadMap(const std::string& filename);
    static Vacuum loadVacuum(const std::string& filename);
};


#endif
