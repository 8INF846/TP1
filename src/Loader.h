#ifndef LOADER_H
#define LOADER_H

#include <string>

#include "MapReal.h"
#include "Vacuum.h"

class Loader {
public:
    /**
    * Create the MapReal object from a file
    * @param filename the path of the file
    * @return the instance of the MapReal object
    */
    static MapReal loadMap(const std::string& filename);

    /**
    * Create the Vacuum object from a file
    * @param filename the path of the file
    * @param map the map to test if the basePos is a real case
    * @return the instance of the Vacuum object
    */
    static Vacuum loadVacuum(const std::string& filename, MapReal& map);
};


#endif
