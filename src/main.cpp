#include <iostream>
#include "Loader.h"

void help() {
    std::cout << "Utilisation : vacuum [map_file_name] [vacuum_file_name]" << std::endl;
}

int main(int argc, char* argv[]) {

    if(argc != 3) {
        help();
        return EXIT_FAILURE;
    }

    // Charger la map et la lancer
    Map map = Loader::loadMap(argv[1]);
    map.start();
    // Charger l'aspirateur, l'ajouter dans la map et le lancer
    Vacuum vacuum = Loader::loadVacuum(argv[2]);
    map.addVacuum(vacuum);
    vacuum.start();
    // Attendre la fin de l'exécution de la map et de l'aspirateur
    map.join();
    vacuum.join();
    return EXIT_SUCCESS;
}
