#ifndef LOADER_H
#define LOADER_H

#include <string>

#include "MapReal.h"
#include "Vacuum.h"

class Loader {
public:
    static MapReal loadMap(const std::string& filename);
    static Vacuum loadVacuum(const std::string& filename);
};


#endif
