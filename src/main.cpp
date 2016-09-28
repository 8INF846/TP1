#include <iostream>
#include <thread>

#include "Loader.h"
#include "Startable.h"

void help() {
    std::cout << "Utilisation : vacuum [map_file_name] [vacuum_file_name]" << std::endl;
}

int main(int argc, char* argv[]) {

    if(argc != 3) {
        help();
        return EXIT_FAILURE;
    }

    // Charger la map et la lancer
    MapReal map = Loader::loadMap(std::string(argv[1]));
    std::cout << map;
    //std::thread threadMap = map.start();
    // Charger l'aspirateur, l'ajouter dans la map et le lancer
    //Vacuum vacuum = Loader::loadVacuum(std::string(argv[2]));
    //std::thread threadVacuum = vacuum.start();
    // Attendre la fin de l'exécution de la map et de l'aspirateur
    //threadMap.join();
    //threadVacuum.join();
    return EXIT_SUCCESS;
}
